#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

// Pines de los LEDs
const int led=3;
const int pot=4; 
int estado=0;
int accion=2;
String boton= "ENCENDER";
float tension=0;
String tensionCambio="X.XX";



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
    body { background-color: rgb(247, 4, 4); text-align: center; font-family: Arial, sans-serif; color: white; }
    .led { width: 80px; height: 45px; background-color: yellow; color: black; border: 2px black; border-radius: 80px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .pantalla{width:160px; height: 90px; border: 1px solid rgb(0, 0, 0); border-radius:10px; background-color: cyan; margin: 4px 4px;}
    button:hover { opacity: 0.8; }
    .centrado{display: flex;
    justify-content: center;}
    .borde{width:170px; height:100px; border:5px black; border-radius: 18px; background-color: black;}
    
  </style>
</head>
<body>

  <h1 class='titulo' style="font-size:32px; color: black; font-family: 'courier', sans-serif; "><i><b>MONITOR</b></i></h1>

<div class="centrado">
  <div class="borde">
  <div class="pantalla">
    <br>
    <p><strong>Tension: __VALOR_TENSION__ V</strong>V</p>

</div>
</div>

</div>
<div class="centrado">
<p><a href='/led'><button class='led'>_TEXTO_</button></a></p>
</div>

</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  for(int i = 0; i < 3; i++) {
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
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
    pagina.replace("_TEXTO_", boton);
    pagina.replace("__VALOR_TENSION__", tensionCambio);
    server.send(200, "text/html", pagina);
  });
   server.on("/led", []() {
    if(estado==1){
        estado=0;
        accion=estado;
        boton="ENCENDER";
    }
    if(estado==0){
        estado=1;
        accion=estado;
        boton="APAGAR";
    }
    
    

    
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
   
  
  
}
void loop(){
tension=map(analogRead(pot),0,1023,0,255);
tensionCambio=tension;
if(accion==0){
    digitalWrite(led,LOW);
    accion=2;
}
if(accion==1){
    digitalWrite(led,HIGH);
    accion=2;
}

server.handleClient();

}