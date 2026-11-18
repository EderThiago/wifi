#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";


int led=27;
int est=0;
String modo="ALTA";
String uso="95%";
String fondop="#2c3e50";
String fondom="red";
t-init=0;
t-delay=500;
t-previo=0;
String cambio;
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
    body { background-color: _FONDOP_; text-align: center; font-family: Arial, sans-serif; color: _LETRA_; }
    .monitor { width: 600px; height:180px; border: 3px solid white ; border-radius: 20px; justify-content: center; margin: 0 auto; background-color:_FONDOM_;}
    .btn-carga{ width: 150px; }
  </style>
</head>

<body>
  <h1 class='titulo' style="font-family: Segoe UI; font: size 34px; color:#ffffff ">MONITOR DE CARGA DEL SISTEMA</h1>

 
  <div class="monitor">
  <p>NIVEL DE CARGA: XXXX</p>
  <p>USO ESTIMADO: YYYY</p>
  </div>

  <p><a href='/alta'><button class='btn-carga' >ALTA</button></a></p>
  <p><a href='/media'><button class='btn-carga' >MEDIA</button></a></p>
  <p><a href='/baja'><button class='btn-carga' >BAJA</button></a></p>
 
 
  
  
 
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
    pagina.replace("YYYY", uso);
    pagina.replace("_FONDOP_", fondop);
    pagina.replace("_FONDOM_", fondom );
    
    
    request->send(200, "text/html", pagina);
  });

  server.on("/alta", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=0;
    modo="ALTA";
    uso="95%";
    fondop=cambio;
    fondom="rojo ";

  });
  server.on("/restringir", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=1;
    modo="MEDIA";
    uso="60%";
    fondop="#2c3e50";
    fondom="orange";
  });
  server.on("/bloquear", HTTP_GET, [](AsyncWebServerRequest *request) {
    est=2;
    modo="BAJA";
    uso="25%";
    fondop="#2c3e50";
    fondom="verde";
  });
  
  // Iniciar servidor
  server.begin();
}

void loop() {

switch(est){
  case 0:
  t-delay=100;
   while(est==0){
  t-init=millis();
  t-previo=t-init;

  while(t-init-t-previo<=t-delay){
    t-init=milis();
    if(onoff){
        digitalWrite(led, LOW);
    }else{
        digitalWrite(led, HIGH);
    }
    if(t-init%2){
        fondop="#2c3e50";
        fondop="#000000";
    }

  }
onoff=!onoff;
}
  break;
  case 1:
  t-delay=400;
   while(est==1){
  t-init=millis();
  t-previo=t-init;

  while(t-init-t-previo<=t-delay){
    t-init=milis();
    if(onoff){
        digitalWrite(led, LOW);
    }else{
        digitalWrite(led, HIGH);
    }

  }
onoff=!onoff;
}
  break;
  case 2:
    digitalWrite(led, LOW);
  break;
}

}