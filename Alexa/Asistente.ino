#include <WiFi.h>
#include <PubSubClient.h>



//BluetoothSerial SerialBT;
//Configuracion de conexión wifi
#define ssid  "commandserver"
#define password  "delivali"
#define mqtt_server  "192.168.1.2"

//Sensor de humedad en el pin 15
#include "DHTesp.h"

#define DHTpin 15    //D15 del ESP32 DevKit

DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {

  Serial.begin(115200);
  dht.setup(DHTpin, DHTesp::DHT11);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

}

void loop() {
   delay(dht.getMinimumSamplingPeriod());
    float temperatura = dht.getTemperature();
    char stempC [10]; 
  if (!client.connected()) {
    reconnect();
  }
  dtostrf(temperatura, 10, 2, stempC);
  client.publish("Weather", stempC);
  delay(3000);
  client.loop();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
