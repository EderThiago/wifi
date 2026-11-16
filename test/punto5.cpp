#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";


int buzzer=27;
String est="SILENCIO";
int frec=0;

// Servidor Asíncrono 
AsyncWebServer server(80);


bool ledEncendido = true;

// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Servidor Web ESP32</title>
  <style>
    body { background-color: black; text-align: center; font-family: Arial, sans-serif; color: white; }
    .est_nota { width:400px; height:80px; border: 2px dashed #000; text-align: center; }
    .centrado{ display:flex; justify-content: center;}
    .btn-color{width: 60px; height: 150px;}
  </style>
</head>

<body>
  <h1 class='titulo' style="font-family:verdana;">GENERADOR DE TONOS</h1>

  <span>Color Actual: XXX</span>
  <div>
  <p>Tocando: XXX</p>
  </div>
  
  <div class='centrado'>
  <p><a href='/toggleDo'><button class='btn-color' >DO</button></a></p>
  <p><a href='/toggleRe'><button class='btn-color' >RE</button></a></p>
  <p><a href='/toggleMi'><button class='btn-color' >MI</button></a></p>
  <p><a href='/toggleLa'><button class='btn-color' >LA</button></a></p>
  <p><a href='/toggleSol'><button class='btn-color' >SOL</button></a></p>
  <p><a href='/toggleApagar'><button class='btn-color' style="background-color: red;">SILENCIO</button></a></p>
  </div>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
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
    
    pagina.replace("XXX", est);
    
    
    request->send(200, "text/html", pagina);
  });

  server.on("/toggleDo", HTTP_GET, [](AsyncWebServerRequest *request) {
    est="DO";
    frec=1;
  });
  server.on("/toggleRe", HTTP_GET, [](AsyncWebServerRequest *request) {
    est="RE";
    frec=2;
  });
  server.on("/toggleMi", HTTP_GET, [](AsyncWebServerRequest *request) {
    est="MI";
    frec=3;
  });
  server.on("/toggleLa", HTTP_GET, [](AsyncWebServerRequest *request) {
    est="LA";
    frec=4;
  });
  server.on("/toggleSol", HTTP_GET, [](AsyncWebServerRequest *request) {
    est="SOL";
    frec=5;
  });
  server.on("/toggleApagar", HTTP_GET, [](AsyncWebServerRequest *request) {
    est="SILENCIO";
    frec=0;
  });
  // Iniciar servidor
  server.begin();
}

void loop() {

switch(frec){
  case 0:
  digitalWrite(buzzer, LOW);
  break;
  case 1:
  tone(buzzer, 261.63);
  break;
  case 2:
  tone(buzzer, 293.66);
  break;
  case 3:
  tone(buzzer, 329.63);
  break;
  case 4:
  tone(buzzer, 440);
  break;
  case 5:
  tone(buzzer, 392);
  break;
}

}