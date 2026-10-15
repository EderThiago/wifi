#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "EL_NOMBRE_DE_TU_WIFI";
const char* password = "LA_CONTRASENA_DE_TU_WIFI";


const int ledPin = 23; 

// Configuro sv asincrono
AsyncWebServer server(80);

bool ledEncendido = true;

// No cambia la página
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Control LED</title>
  <style>
    body { background-color: red; text-align: center; padding-top: 50px; }
    .titulo { font-family: 'Courier', monospace; font-weight: bold; font-style: italic; font-size: 32px; color: white; }
    .boton { width: 120px; height: 60px; background-color: yellow; color: black; border: 2px solid black; border-radius: 10px; font-size: 18px; font-weight: bold; cursor: pointer; margin-top: 20px;}
  </style>
</head>
<body>
  <h1 class='titulo'>ENCENDIDO DE LED INTERNO</h1>
  <p><a href='/toggle'><button class='boton'>__BOTON_TEXTO__</button></a></p>
</body>
</html>
)rawliteral";


// Funciones dedicadas para cada ruta (/...)

// Función para manejar la petición a la página principal (/)
void handleRootRequest(AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original
    //parte para cambiar lo que dice el boton una vez que se prende o apaga
    String textoBoton;
    if (ledEncendido == true)  textoBoton = "ON";
    else textoBoton = "OFF";
    //pagina.replace busca el texto y lo reemplaza con lo que pongas. 
    pagina.replace("__BOTON_TEXTO__", textoBoton);
    
    request->send(200, "text/html", pagina);
}

// Función para manejar la petición de toggle (/toggle)
void handleToggleRequest(AsyncWebServerRequest *request) {
    digitalWrite(ledPin,ledEncendido);
    ledEncendido = !ledEncendido;
    request->redirect("/"); //lo mando a la ruta (/)
}


void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Conexion wifi como antes
 int timeout = 20; 
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Dirección IP: http://");
  Serial.println(WiFi.localIP());

  // defino las rutas para las request 
  // Ahora ponemos el nombre de nuestra función dedicada
  server.on("/", HTTP_GET, handleRootRequest);
  server.on("/toggle", HTTP_GET, handleToggleRequest);

  // Iniciar servidor
  server.begin();
}

void loop() {
  // El loop sigue libre para las tareas de hardware

}