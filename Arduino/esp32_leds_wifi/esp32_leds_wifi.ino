#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "educasor";
const char* password = "12345678e";
//WebServer server(80);

WebServer server(80);

uint8_t pinD4 = 4;
bool estadoPinD4 = LOW;

uint8_t pinD15 = 15;
bool estadoPinD15 = LOW;

void setup() {
  Serial.begin(115200);
  delay(50);
  pinMode(pinD4, OUTPUT);
  pinMode(pinD15, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Verificar estado de la conexión
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("IP: ");  
  Serial.println(WiFi.localIP());

  server.on("/", controlarIndex);
  server.on("/pinD4on", controlarPinD4on);
  server.on("/pinD4off", controlarPinD4off);
  server.on("/pinD15on", controlarPinD15on);
  server.on("/pinD15off", controlarPinD15off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(estadoPinD4)
    {digitalWrite(pinD4, HIGH);}
  else
    {digitalWrite(pinD4, LOW);}
  
  if(estadoPinD15)
    {digitalWrite(pinD15, HIGH);}
  else
    {digitalWrite(pinD15, LOW);}
}

void controlarIndex() {
  estadoPinD4 = LOW;
  estadoPinD15 = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", escribirHTML(estadoPinD4,estadoPinD15)); 
}

void controlarPinD4on() {
  estadoPinD4 = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", escribirHTML(true,estadoPinD15)); 
}

void controlarPinD4off() {
  estadoPinD4 = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", escribirHTML(false,estadoPinD15)); 
}

void controlarPinD15on() {
  estadoPinD15 = HIGH;
  Serial.println("GPIO15 Status: ON");
  server.send(200, "text/html", escribirHTML(estadoPinD4,true)); 
}

void controlarPinD15off() {
  estadoPinD15 = LOW;
  Serial.println("GPIO15 Status: OFF");
  server.send(200, "text/html", escribirHTML(estadoPinD4,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Error! Not found");
}

String escribirHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html lang=\"es\"> <html>\n";
  ptr +="<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title> Control de LED</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".buttonR-on {background-color: red;}\n";
  ptr +=".buttonG-on {background-color: springgreen;}\n";
  ptr +=".button-on:active {background-color: #3498db;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Servidor ESP32</h1>\n";
  ptr +="<h3>Configurado en modo estación (STA)</h3>\n";
  
   if(led1stat)
    {ptr +="<p>LED1 (pin4) Status: ON</p><a class=\"button button-off\" href=\"/pinD4off\">OFF</a>\n";}
  else
    {ptr +="<p>LED1 (pin4) Status: OFF</p><a class=\"button buttonR-on\" href=\"/pinD4on\">ON</a>\n";}

  if(led2stat)
    {ptr +="<p>LED2 (pin15) Status: ON</p><a class=\"button button-off\" href=\"/pinD15off\">OFF</a>\n";}
  else
    {ptr +="<p>LED2 (pin15) Status: OFF</p><a class=\"button buttonG-on\" href=\"/pinD15on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
