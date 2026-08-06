#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

const int ledPin [5] = {0,1,2,3,4}; 
const int anim1[3][5]={
                {1,0,0,0,1},
                {0,1,0,1,0},
                {0,0,1,0,0}
};
const int anim2[2][5]={
                {1,0,1,0,1},
                {0,1,0,1,0}
};
int anim=0;
int estled=0;
int t_actual;
int t_previo=0;
int t_delay;
WebServer server(80);

bool ledEncendido = true;

// pagina
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8'>
  <title>Servidor Web ESP32</title>
  <style>
    body { background-color: black; text-align: center; }
    .anim1 { width: 80px; height: 45px; background-color: blue; color: black; border: 2px solid black; border-radius: 80px; font-size: 14px; font-weight: bold; cursor: pointer; }
    .anim2 { width: 80px; height: 45px; background-color: yellow; color: black; border: 2px solid black; border-radius: 80px; font-size: 14px; font-weight: bold; cursor: pointer; }
    .off { width: 80px; height: 45px; background-color: black; color: black; border: 2px solid black; border-radius: 80px; font-size: 14px; font-weight: bold; cursor: pointer; }
  </style>
</head>
<body>
  <h1 class='titulo'>BLINK DE LED INTERNO</h1>
  <p><a href='/toggleAnim1'><button class='anim1'>ANIMACIÓN 1</button></a></p><br
  <p><a href='/toggleAnim2'><button class='anim2'>ANIMACIÓN 2</button></a></p>
  <p><a href='/toggleOff'><button class='off'>APAGAR</button></a></p>
</body>
</html>
)rawliteral";




void setup() {
  
  Serial.begin(115200);
  for(int i=0;i<5;i++){
  pinMode(ledPin[i], OUTPUT);
                        }
  // conexion al wifi como antes
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeout = 20; // 20 intentos de 500ms = 10 segundos
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

  
   server.on("/", []() {
    
   

    server.send(200, "text/html", pagina_template);
  });
  

  
  server.on("/toggleAnim1", []() {
    
    anim=1;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", ""); // 302 y send Header para que no me quede en /header
  });

  server.on("/toggleAnim2", []() {
    
    anim=2;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", ""); // 302 y send Header para que no me quede en /header
  });

  server.on("/toggleOff", []() {
    
    anim=0;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", ""); // 302 y send Header para que no me quede en /header
  });
  server.begin();
}

void loop() {
  server.handleClient();
  
    if(anim==1){
        estled=0;

        for(int i=0;i<3;i++){
            for(int j=0; j<5;j++){
            digitalWrite(ledPin[j], anim1[estled][j]);
            }
        }

        if(estled==2){
            estled=0;
        }else{
            estled++;
        }
        
        t_actual=millis();
        t_delay=millis()+1000;
        while(t_actual-t_previo<=t_delay){
            t_actual=millis();
        }
  
  }


  if(anim==2){
        estled=0;
    while(anim==2){

        for(int i=0;i<3;i++){
            for(int j=0; j<5;j++){
            digitalWrite(ledPin[j], anim2[estled][j]);
            }
        }

        if(estled==1){
            estled=0;
        }else{
            estled++;
        }

        t_actual=millis();
        t_delay=millis()+1000;
        while(t_actual-t_previo<=t_delay){
            t_actual=millis();
        }
  }
  }

    if(anim==0){
        for(int i=0;i<5;i++){
            digitalWrite(ledPin[i],LOW);
        }
        while(anim==0){}
     }
}