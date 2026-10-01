#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

// Pines de los LEDs
const int d1[7] = {32,33, 25, 26, 27,12,13}; 
const int d2[7] = {14,15,4,2,5,18,19};

// Animaciones
const int seg[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1},
};


int display = 1;
String texto1= "-";
String texto2="ANIMACIÓN2";

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
    .teclado { width: 120px; height: 45px; background-color: rojo; color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .num { width: 120px; height: 45px; background-color: rgb(245, 9, 9); color: black; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .d1 { width: 120px; height: 45px; background-color: rgb(38, 0, 255); color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    .d2 { width: 120px; height: 45px; background-color: rgb(0, 255, 42); color: white; border: none; border-radius: 8px; font-size: 14px; font-weight: bold; cursor: pointer; margin: 10px; }
    button:hover { opacity: 0.8; }
    .centrado{display: flex;
    justify-content: center;}
  </style>
</head>
<body>

  <h1 class='titulo'>CONTROL DE DISPLAYS</h1>

<div class="centrado">
  <table border="0">
        <thead>
            <tr>
                <td ><p><a href='/toggled1'><button class='d1'>ANIMACIÓN 1</button></a></p></td>
                <td ><p><a href='/toggled2'><button class='d2'>ANIMACIÓN 2</button></a></p></td>
            </tr>
        </thead>
        
    </table>
</div>

<div class="centrado">
   <table border="0">
        
        <tbody>
            <tr>
                <td><p><a href='/toggle1'><button class='num'>1</button></a></p></td>
                <td><p><a href='/toggle2'><button class='num'>2</button></a></p></td>
                <td><p><a href='/toggle3'><button class='num'>3</button></a></p></td>
               
            </tr>
            <tr>
                <td><p><a href='/toggle4'><button class='num'>4</button></a></p></td>
                <td><p><a href='/toggle5'><button class='num'>5</button></a></p></td>
                <td><p><a href='/toggle6'><button class='num'>6</button></a></p></td>
               
            </tr>
            <tr>
                <td><p><a href='/toggle7'><button class='num'>7</button></a></p></td>
                <td><p><a href='/toggle8'><button class='num'>8</button></a></p></td>
                <td><p><a href='/toggle9'><button class='num'>9</button></a></p></td>
               
            </tr>
            <tr>
                <td></td>
                <td><p><a href='/toggle0'><button class='num'>0</button></a></p></td>

            </tr>
        </tbody>
    </table>
</div>
  
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  for(int i = 0; i < 5; i++) {
    pinMode(d1[i], OUTPUT);
    digitalWrite(d1[i], LOW);
    pinMode(d2[i], OUTPUT);
    digitalWrite(d2[i], LOW); // Asegurar que inicien apagados
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
    pagina.replace("ANIMACIÓN 1", texto1);
    pagina.replace("ANIMACIÓN 2", texto2);
    server.send(200, "text/html", pagina);
  });

  server.on("/toggled1", []() {
    display=1;
    texto1="-";
    texto2="ANIMACIÓN 2";


    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/toggled2", []() {
    display=2;
    texto1="ANIMACIÓN 1";
    texto2="-";
    

    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/toggle0", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[0][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[0][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle1", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[1][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[1][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle2", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[2][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[2][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle3", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[3][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[3][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle4", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[4][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[4][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle5", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[5][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[5][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle6", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[6][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[6][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle7", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[7][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[7][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle8", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[8][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[8][i]);
        }
    }
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });
  server.on("/toggle9", []() {
    if(display==1){
        for(int i=0;i<7;i++){
            digitalWrite(d1[i], seg[9][i]);
        }
    }else{
        for(int i=0;i<7;i++){
            digitalWrite(d2[i], seg[9][i]);
        }
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

  
  
}
