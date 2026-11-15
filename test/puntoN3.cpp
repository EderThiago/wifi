#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

// Pines de los LEDs
const int rojo=27;
const int verde=32;
const int azul=14;
int color=0;



WebServer server(80);

// Variables para el control de tiempo sin bloquear (no-blocking delays)
unsigned long tiempoAnterior = 0;
const long intervalo = 300; // Velocidad de la animación en milisegundos
int pasoActual = 0;

// Página HTML
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Servidor Web ESP32</title>
  <style>
    body { background-color: black; text-align: center; font-family: Arial, sans-serif; color: white; }
    .centrado { display:flex; justify-content:center;  }
    .btn-color{border-radius: 50%; width: 80px; height: 80px; border:3px solid white;}
  </style>
</head>
<body>
  <h1 class='titulo'>Selector de Color RGB</h1>

  <span>Color Actual: XXX</span>

  <div class='centrado'>
  <p><a href='/toggleRojo'><button class='btn-color' style="background-color: red; ">ROJO</button></a></p>
  <p><a href='/toggleVerde'><button class='btn-color' style="background-color: green;">VERDE</button></a></p>
  <p><a href='/toggleAzul'><button class='btn-color' style="background-color: blue;">AZUL</button></a></p>
  <p><a href='/toggleBlanco'><button class='btn-color' style="background-color: white;">BLANCO</button></a></p>
  <p><a href='/toggleApagar'><button class='btn-color' style="background-color: gray;">APAGADO</button></a></p>
  </div>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  pinMode(rojo,OUTPUT);
  pinMode(azul,OUTPUT);
  pinMode(verde,OUTPUT);

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
    server.send(200, "text/html", pagina_template);
  });

  server.on("/toggleRojo", []() {
    color=1;
    pagina.replace("XXX", "ROJO");

    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/toggleVerde", []() {
    pagina.replace("XXX", "VERDE");
    color=2;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggleAzul", []() {
    color=3;
    pagina.replace("XXX", "AZUL");
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggleBlanco", []() {
    color=4;
    pagina.replace("XXX", "BLANCO");
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/toggleApagado", []() {
   color=0;
   pagina.replace("XXX", "APAGADO");
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  // SOLUCIÓN AL ERROR: Ignorar o responder vacío al favicon
  server.on("/favicon.ico", []() {
    server.send(204, "text/plain", ""); // 204 significa "Sin contenido", limpia el error
  });

  // Manejar cualquier otra ruta desconocida de forma limpia
  server.onNotFound([]() {
    server.send(404, "text/plain", "Ruta no encontrada");
  });

  server.begin();
}

void loop() {
  server.handleClient();
 switch(color){
    case 0:
    digitalWrite(rojo, HIGH);
    digitalWrite(verde, LOW);
    digitalWrite(azul, LOW);
    break;
    case 1:
     digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    digitalWrite(azul, LOW);
    break;
    case 2:
     digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    digitalWrite(azul, LOW);
    break;
    case 3:
     digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    digitalWrite(azul, LOW);
    break;
    case 4:
     digitalWrite(rojo, LOW);
    digitalWrite(verde, LOW);
    digitalWrite(azul, LOW);
    break;
 }
 
  
}
