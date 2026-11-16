#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";


int led=27;
int est=0;
String modo="DETENIDO";
String tem="--C";
String ven="APAGADO";
String fondo="grey";
String color="black";

// Servidor Asíncrono 
AsyncWebServer server(80);




// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(


<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Servidor Web ESP32</title>
  <style>
    body { background-color: #ecf0f1; text-align: center; font-family: Arial, sans-serif; color: _LETRA_; }
    .panel_temp { width: 550px; padding:30px; border-radius: 20px ; justify-content: center;  margin: 0 auto}
    .centrado{ display:flex; justify-content: center;}
  </style>
</head>

<body>
  <h1 class='titulo' style="font-family:Arial-black; font: size 34px;">SISTEMA DE REFRIGERACIÓN</h1>

 
  <div class="panel_temp">
  <p >MODO ACTUAL: XXXX</p>
  <p>TEMPERATURA OBJETIVO: YYYY</p>
  <p>“ESTADO DEL VENTILADOR: ZZZZ”</p>
  </div>
 
  
  <p><a href='/detenido'><button class='panel_temp' >DETENIDO</button></a></p>
  <p><a href='/ventilacion'><button class='panel_temp' >VENTILACIÓN</button></a></p>
  <p><a href='/intensivo'><button class='panel_temp' >INTENSIVO</button></a></p>
 
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
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
    
    pagina.replace("XXXX", modo);
    pagina.replace("YYYY", tem);
    pagina.replace("ZZZZ", ven);
    pagina.replace("_FONDO_", fondo);
    pagina.replace("_COLOR_", color );
    
    
    request->send(200, "text/html", pagina);
  });

  server.on("/detenido", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=0;
    modo="DETENIDO";
    tem="--C";
    ven="APAGADO";
    fondo="grey";
    color="black"

  });
  server.on("/ventilacion", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=1;
    modo="VENTILACION";
    tem="25C";
    ven="VELOCIDAD MEDIA";
    fondo="light blue";
    color="black"
  });
  server.on("/intensivo", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=2;
    modo="ENFRIAMIENTO INTENSIVO";
    tem="18C";
    ven="VELOCIDAD ALTA";
    fondo="white";
    color="white"
  });
  
  // Iniciar servidor
  server.begin();
}

void loop() {

switch(est){
  case 0:
  digitalWrite(led, LOW);
  break;
  case 1:
  tone(led, 1,66);
  break;
  case 2:
  digitalWrite(led, HIGH);  
  break;
}

}