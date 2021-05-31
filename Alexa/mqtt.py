import random

from paho.mqtt import client as mqtt_client

class Mqtt (mqtt_client.Client):
 
    global broker 
    broker= '192.168.1.2'
    global port
    port = 1883
    global topic 
    topic= "Commands"
    # Genera un client ID con  un prefijo al Azar
    global client_id 
    client_id = f'python-mqtt-{random.randint(0, 100)}'
    # username = 'emqx'
    # password = 'public'


    def connect_mqtt() -> mqtt_client:
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Ready to receive Commands")
            else:
                print("Failed to connect, return code %d\n", rc)

        client = mqtt_client.Client(client_id)

        client.on_connect = on_connect
        client.connect(broker, port)
        return client

    def connect_mqtt2() -> mqtt_client:
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to Weather Channel")
            else:
                print("Failed to connect, return code %d\n", rc)

        client = mqtt_client.Client(client_id)

        client.on_connect = on_connect
        client.connect(broker, port)
        return client


    def subscribe(self,client: mqtt_client):
        def on_message(client, userdata, msg):
            print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
            self.Mensaje=msg.payload.decode()
        client.subscribe(topic)
        client.on_message = on_message
        


    def run(self):
        client = connect_mqtt()
        subscribe(client)
        client.loop_forever()

