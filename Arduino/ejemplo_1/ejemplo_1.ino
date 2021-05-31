#include <WiFi.h>

const char* ssid = "Red chida";
const char* password = "12345678";

void setup()
{
  Serial.begin (115200);
  delay (10);

  // Se inicia la conexión
  Serial.println ();
  Serial.println();
  Serial.print ("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin (ssid, password);
  // Se verifica se realiza la conexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print (".");
  }
  // lograda la conexion se muestra la informacion
  Serial.println ("");
  Serial.println ("WiFi connected");
  Serial.println ("IP address: ");
  Serial.println ( WiFi.localIP() );
}

void loop(){

}
