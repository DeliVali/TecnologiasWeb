#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>
#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

//Creacion de conexion wifi y servidor
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "educasor";
const char* password = "12345678e";
//WebServer server(80);

WebServer server(80);


//Sensor de humedad en el pin 15
#include "DHTesp.h"

#define DHTpin 15    //D15 del ESP32 DevKit

DHTesp dht;





void setup() {
  // put your setup code here, to run once:
  
 Serial.begin(115200);
  delay(50);
dht.setup(DHTpin, DHTesp::DHT11);
Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  //Funcion para intentar conectar a wifi
 while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }


  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("IP: ");  
  Serial.println(WiFi.localIP());
  server.on("/", controlarIndex);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");



}

void loop() {
  // put your main code here, to run repeatedly:
    delay(dht.getMinimumSamplingPeriod());


  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  
  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("No se pudo leer sensor DHT!");
    return;
  }





  
  server.handleClient();

}



void handle_NotFound(){
  server.send(404, "text/plain", "Error! Not found");
}

void controlarIndex() {
  server.send(200, "text/html", escribirHTML()); 
}

String escribirHTML(){



  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  
    String ptr ="<!DOCTYPE html>";
  ptr+="<html lang='en'>";
   ptr+= "<head>";
     ptr+="<meta charset='UTF-8'>";
    ptr+= "<meta http-equiv='X-UA-Compatible' content='IE=edge'>";
    ptr+= "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    ptr+= "<title>Weather report</title>";
    
   ptr+="<style>";
  ptr+="table,";
  ptr+="table td {";
    ptr+="border: 1px solid #cccccc;}";

  ptr+="td {";
    ptr+="height: 80px;";
   ptr+= "width: 160px;";
    ptr+="text-align: center;";
    ptr+="vertical-align: middle;}";
     ptr+= "</style>";


    
 ptr+="</head>";
 ptr+="<body>";
    ptr+= "<table style='width:100%'>";
    ptr+= "<tr>";
    ptr+= "<th>Estado</th>";
    ptr+= "<th>Humedad</th>";
    ptr+= "<th>Temperatura (C)</th>";
   ptr+=  "<th>Temperatura (F)</th>";
    ptr+= "<th>Indice de calor (C)</th>";
   ptr+=  "<th>Indice de calor (F)</th>";
   ptr+=  "</tr>";


   ptr+=  "<tr>";

    ptr+=  "<td>";
    ptr+= dht.getStatusString();
                ptr+=  "</td>";
                
  ptr+=   "<td>";
          ptr+= humedad;
  ptr+= "</td>";
  
   ptr+=  "<td>";
    ptr+= temperatura;
    ptr+= "</td>";
    
    ptr+= "<td>";
    ptr+= dht.toFahrenheit(temperatura);
     ptr+= "</td>";
     
   ptr+=  "<td>";
   ptr+= dht.computeHeatIndex(temperatura, humedad, false);
    ptr+= "</td>";

     ptr+=  "<td>";
   ptr+=dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true);
    ptr+= "</td>";
    
 ptr+=    "</tr>";
   
    

   ptr+=  "</table>";
 ptr+="</body>";
 ptr+="</html>";
 
  return ptr;

}
