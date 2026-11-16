#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";


int pul[3]={27,32,14};

String fondo1="red"; 
String fondo2="red"; 
String fondo3="red"; 
String est1="SUELTO"; 
String est2="SUELTO"; 
String est3="SUELTO"; 
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
    body { background-color: #f0f0f0; text-align: center; font-family: Arial, sans-serif; color: white; }
    .linea-de-estado { width:300px; margin: 10px; padding: 15px; border:2px solid black  }
    
  </style>

</head>
<body>
  <h1 class='titulo' style="font-family: monospace;">MONITOR DE ENTRADAS DIGITALES</h1>

  <span>Color Actual: XXX</span>


  <ol style=" list-style-type: none; padding: 0;">
    <li style="_FONDO1_">ENTRADA 1: YYYY</li>
    <li style="_FONDO2_">ENTRADA 2: YYYY</li>
    <li style="_FONDO3_">ENTRADA 3: YYYY</li>
  </ol>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  for(int i=0; i<3; i++){
    pinMode(pul[i], INPUT);
  }



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
    
    pagina.replace("_FONDO1_", fondo1);
    pagina.replace("_FONDO2_", fondo2);
    pagina.replace("_FONDO3_", fondo3);
    pagina.replace("ENTRADA 1: YYYY", "ENTRADA 1" + est1);
    pagina.replace("ENTRADA 2: YYYY", "ENTRADA 2" + est2);
    pagina.replace("ENTRADA 3: YYYY", "ENTRADA 3" + est3);
    
    
    request->send(200, "text/html", pagina);
  });


  // Iniciar servidor
  server.begin();
}

void loop() {
if(digitalRead(pul[1])){
    fondo1="green";
    est1="APRETADO";
}else{
    fondo1="red";
    est1="SUELTO";
}
if(digitalRead(pul[2])){
    fondo2="green";
    est2="APRETADO";
}else{
    fondo2="red";
    est2="SUELTO";
}
if(digitalRead(pul[3])){
    fondo3="green";
    est3="APRETADO";
}else{
    fondo3="red";
    est3="SUELTO";
}

}