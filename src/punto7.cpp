#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";


int ver=27;
int roj=32;
int est=0;
String modo="ACCESO PERMITIDO";
String fis="VERDE";
String fondo="green";
String color="white";
t-init=0;
t-delay=500;
t-previo=0;
bool onoff=false;
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
    body { background-color: #bdc3c7; text-align: center; font-family: Arial, sans-serif; color: _LETRA_; }
    .panel_acceso { width: 70%; padding:35px; border-radius: 20px ; background-color: _FONFO_}
    .btn-acceso{ width: 170px; height:55px;}
  </style>
</head>

<body>
  <h1 class='titulo' style="font-family:monospace; font: size 36px;">CONTROL DE ACCESO</h1>

 
  <div class="panel_acceso">
  <p>ESTADO DE ACCESO: XXXX</p>
  <p>INDICADOR FÍSICO: YYYY</p>


  <p><a href='/permitir'><button class='btn-acceso' >PERMITIR</button></a></p>
  <p><a href='/restringir'><button class='btn-acceso' >RESTRINGIR</button></a></p>
  <p><a href='/bloquear'><button class='btn-acceso' >BLOQUEAR</button></a></p>
  </div>
 
  
  
 
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(ver, OUTPUT);
  pinMode(roj, OUTPUT);
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
    pagina.replace("YYYY", fis);
    pagina.replace("_FONDO_", fondo);
    pagina.replace("_COLOR_", color );
    
    
    request->send(200, "text/html", pagina);
  });

  server.on("/permitir", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=0;
    modo="ACCESO PERMITIDO";
    fis="VERDE";
    fondo="green";
    color="white"

  });
  server.on("/restringir", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=1;
    modo="ACCESO RESTRINGIDO";
    fis="ALTERNANDO";
    fondo="yellow";
    color="black"
  });
  server.on("/bloquear", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=2;
    modo="BLOQUEO TOTAL";
    fis="ROJO";
    fondo="rojo";
    color="blanco"
  });
  
  // Iniciar servidor
  server.begin();
}

void loop() {

switch(est){
  case 0:
  digitalWrite(roj, LOW);
  digitalWrite(ver, HIGH);
  break;
  case 1:
  while(est==1){
  t-init=millis();
  t-previo=t-init;

  while(t-init-t-previo<=t-delay){
    t-init=milis();
    if(onoff){
        digitalWrite(roj, LOW);
        digitalWrite(ver, HIGH);
    }else{
        digitalWrite(roj, HIGH);
        digitalWrite(ver, LOW);
    }

  }
onoff=!onoff;
}
  break;
  case 2:
    digitalWrite(roj, HIGH);
    digitalWrite(ver, LOW);
  break;
}

}