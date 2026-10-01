#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

// Pines de los LEDs
const int ledPin[5] = {32,33, 25, 26, 27}; 

// Animaciones
const int anim1[3][5] = {
  {1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 1, 0, 0}
};
const int anim2[2][5] = {
  {1, 0, 1, 0, 1},
  {0, 1, 0, 1, 0}
};

int animacion = 0;
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
    .anim1 { width: 120px; height: 45px; background-color: blue; color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .anim2 { width: 120px; height: 45px; background-color: yellow; color: black; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .off { width: 120px; height: 45px; background-color: red; color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    button:hover { opacity: 0.8; }
  </style>
</head>
<body>
  <h1 class='titulo'>CONTROL DE ANIMACIONES LED</h1>
  <p><a href='/toggleAnim1'><button class='anim1'>ANIMACIÓN 1</button></a></p>
  <p><a href='/toggleAnim2'><button class='anim2'>ANIMACIÓN 2</button></a></p>
  <p><a href='/toggleApagar'><button class='off'>APAGAR</button></a></p>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  for(int i = 0; i < 5; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], LOW); // Asegurar que inicien apagados
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
    server.send(200, "text/html", pagina_template);
  });

  server.on("/toggleAnim1", []() {
    animacion = 1;
    pasoActual = 0;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/toggleAnim2", []() {
    animacion = 2;
    pasoActual = 0;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/toggleApagar", []() {
    animacion = 0;
    // Apagar todos los LEDs inmediatamente
    for(int i = 0; i < 5; i++) {
      digitalWrite(ledPin[i], LOW);
    }
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

  // --- LÓGICA DE LAS ANIMACIONES (MÁQUINA DE ESTADOS) ---
  unsigned long tiempoActual = millis();

  if (animacion == 1) {
    if (tiempoActual - tiempoAnterior >= intervalo) {
      tiempoAnterior = tiempoActual;
      
      // Aplicar el estado actual de la animación 1 a los pines
      for (int i = 0; i < 5; i++) {
        digitalWrite(ledPin[i], anim1[pasoActual][i]);
      }
      
      // Avanzar al siguiente paso de la matriz (tiene 3 pasos)
      pasoActual = (pasoActual + 1) % 3;
    }
  } 
  else if (animacion == 2) {
    if (tiempoActual - tiempoAnterior >= intervalo) {
      tiempoAnterior = tiempoActual;
      
      // Aplicar el estado actual de la animación 2 a los pines
      for (int i = 0; i < 5; i++) {
        digitalWrite(ledPin[i], anim2[pasoActual][i]);
      }
      
      // Avanzar al siguiente paso de la matriz (tiene 2 pasos)
      pasoActual = (pasoActual + 1) % 2;
    }
  }
}
