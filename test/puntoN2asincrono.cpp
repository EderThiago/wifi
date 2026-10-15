#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";


const int monitor=32;
String fondo;
float luz;
String luzstring;
int valorpin;


// Servidor Asíncrono 
AsyncWebServer server(80);


bool ledEncendido = true;

// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <meta http-equiv="refresh" content="1">
  <title>Control LED</title>
  <style>
    body { background-color: __FONDO__; text-align: center; padding-top: 50px; }
    .titulo { font-family: 'Courier', monospace; font-weight: bold; font-style: italic; font-size: 32px; color: white; }
    .boton { width: 120px; height: 60px; background-color: yellow; color: black; border: 2px solid black; border-radius: 10px; font-size: 18px; font-weight: bold; cursor: pointer; margin-top: 20px;}
    .lectura{border: 3px dotted rgb(250, 248, 248); border-radius: 20px; padding: 25px;}
  </style>
</head>
<body>
  <h1 class='titulo' style="font-family: 'Segoe UI'">MONITOR DE LUZ AMBIENTAL</h1>
  <div class="lectura">
    <span>Nivel de Luz: <span style="font-size:40px"><b>XXX%</b></span></span>
  </div>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(monitor, INPUT);
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


  // Defino rutas de cada peticion para el sv asíncrono
  // La función ahora recibe un puntero a la petición (request)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original
    //parte para cambiar lo que dice el boton una vez que se prende o apaga
    
    //pagina.replace busca el texto y lo reemplaza con lo que pongas. 
    
    pagina.replace("__FONDO__", fondo);
    pagina.replace("XXX%", luzstring);
    
    request->send(200, "text/html", pagina);
  });


  // Iniciar servidor
  server.begin();
}

void loop() {
valorpin=analogRead(monitor);
luz=(valorpin*100)/4095;

luzstring=luz;
if(luz<(33)){
    fondo="blue";
}
if((luz>=33)&&(luz<=(66))){
    fondo="orange";
}
if(luz>66){
    fondo="yellow";
}


}