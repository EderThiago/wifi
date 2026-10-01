#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

// Pines de los LEDs
const int leds[3] = {4,5,6}; 
// Animaciones
const int semaforo[3][3] = {
  {1,0,0},
  {0,1,0},
  {0,0,1}
};

const int emergencia[2][3] = {
  {1,1,0},
  {0,0,0}
};
const int estado=0;



WebServer server(80);

// Variables para el control de tiempo sin bloquear (no-blocking delays)
unsigned long tprevio = 0;
const long tdelay = 300; // Velocidad de la animación en milisegundos
int tactual = 0;

// Página HTML
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Servidor Web ESP32</title>
  <style>
    body { background-color: rgb(247, 244, 244); text-align: center; font-family: Arial, sans-serif; color: white; }
    .lento { width: 200px; height: 200px; background-color: rgb(245, 9, 9); color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .emergencia { width: 200px; height: 200px; background-color: rgb(255, 81, 0); color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .rapido { width: 200px; height: 200px; background-color: rgb(0, 255, 42); color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .normal { width: 200px; height: 200px; background-color: rgb(251, 255, 0); color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }

    button:hover { opacity: 0.8; }
    .centrado{display: flex;
    justify-content: center;}
  </style>
</head>
<body>

  <h1 class='titulo' style="font-size:45px; color: black; font-family: 'Impact', sans-serif; "><u>SEMÁFORO</u></h1>

<div class="centrado">
  <table border="0">
        <tbody>
            <tr>
                <td ><p><a href='/lento'><button class='lento'>LENTO</button></a></p></td>
            </tr>
            <tr>
                <td ><p><a href='/normal'><button class='normal'>NORMAL</button></a></p></td>
            </tr>
            <tr>
                <td ><p><a href='/rapido'><button class='rapido'>RÁPIDO</button></a></p></td>
            </tr>
            <tr>
                <td ><p><a href='/emergencia'><button class='emergencia'>EMERGENCIA</button></a></p></td>

            </tr>
        </tbody>
        
    </table>
</div>


  
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  for(int i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
    // Asegurar que inicien apagados
  }

  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

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

  // --- RUTAS DEL SERVIDOR WEB ---
  server.on("/", []() {
    String pagina = pagina_template;
    server.send(200, "text/html", pagina);
  });
   server.on("/lento", []() {
    estado=1;


    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
   server.on("/normal", []() {
    estado=2;


    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
   server.on("/rapido", []() {
    estado=3;


    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
   server.on("/emergencia", []() {
    estado=4;


    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
}
void loop(){
    server.handleClient();

  while(estado==1){
    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[1][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=70000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }

    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[2][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=3000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }

    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[3][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=50000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }
  }




  while(estado==2){
    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[1][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=45000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }

    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[2][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=2000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }

    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[3][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=65000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }
  }



  while(estado==3){
    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[1][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=20000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }

    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[2][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=1000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }

    for(int i=0;i<3;i++){
        digitalWrite(led[i],semaforo[3][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=40000;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }
  }


  while(estado==4){
    for(int i=0;i<3;i++){
        digitalWrite(led[i],emergencia[1][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=1500;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }

    for(int i=0;i<3;i++){
        digitalWrite(led[i],emergencia[2][i]);
    }

    tactual=millis();
    tprevio=tactual;
    tdelay=1500;
    while(tactual-tprevio<=tdelay){
        tactual=millis();
    }
  }
}