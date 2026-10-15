#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "CESJT";
const char* password = "itisjtsmg";


const int ven = 4; 
int vel=0;
String modo="APAGADO";

// Servidor Asíncrono 
AsyncWebServer server(80);


bool ledEncendido = true;

// HTML -> no cambia
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Control LED</title>
  <style>
    body { background-color: #ecf0f1; text-align: center; padding-top: 50px; }
    .centrado{display: flex;
    justify-content: center;}
    .titulo { font-family: 'Courier', monospace; font-weight: bold; font-style: italic; font-size: 32px; color: darkblue; }
    .boton { width: 180px; height: 60px; color: black; border: 1px solid black; border-radius: 15px; font-size: 18px; font-weight: bold; cursor: pointer; margin-top: 20px;}
  </style>
</head>
<body>
  <h1 class='titulo'>CONTROL DE VENTILADOR</h1>
  <span><a href='/toggle'><button class='boton'><b>__XXXX__</b></span>
    <br>
    <br>
    <br> 
  <div class="centrado">
  <table border="0">
        <tbody>
            <tr>
                <td ><p><a href='/apagado'><button class="boton" style="background-color:red;">APAGADO</button></a></p></td>
            </tr>
            <tr>
                <td ><p><a href='/low'><button class="boton" style="background-color: lightblue;">LOW</button></a></p></td>
            </tr>
            <tr>
                <td ><p><a href='/medium'>  <button class="boton" style="background-color:green;">MEDIUM</button></a></p></td>
            </tr>
            <tr>
                <td ><p><a href='/high'>  <button class="boton" style="background-color:orange;">HIGH</button>
</a></p></td>

            </tr>
        </tbody>
        
    </table>
</div>
  
  
  
  
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(ven, OUTPUT);
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
    pagina.replace("__XXXX__", modo);
    
    request->send(200, "text/html", pagina);
  });

  server.on("/apagado", HTTP_GET, [](AsyncWebServerRequest *request) {
    vel=0;
    modo="APAGADO";
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/apagado", HTTP_GET, [](AsyncWebServerRequest *request) {
    vel=1023/3;
    modo="LOW";
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/apagado", HTTP_GET, [](AsyncWebServerRequest *request) {
    vel=(1023/3)*2;
    modo="MEDIUM";
    request->redirect("/"); //lo mando a la ruta (/)
  });
  server.on("/apagado", HTTP_GET, [](AsyncWebServerRequest *request) {
    vel=1023;
    modo="HIGH";
    request->redirect("/"); //lo mando a la ruta (/)
  });

  // Iniciar servidor
  server.begin();
}

void loop() {
  // Ya no necesitamos server.handleClient(). El servidor asíncrono
  // maneja las peticiones en segundo plano por su cuenta.
  // El loop ahora se dedica 100% a las tareas de hardware.
  analogWrite(ven,vel);

}