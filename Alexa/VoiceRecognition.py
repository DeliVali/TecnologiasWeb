# Paquete que reconoce el audio y lo pasa a texto 
import speech_recognition as sr
#Paquete que convierte el texto a audio
import pyttsx3
#Paquete que redirige a youtube ,wikipedia o manda mensajes a whatssap
import pywhatkit
#Paquete para obtener el dia o la fecha
import datetime
#Paquete para buscar en wikipedia
import wikipedia
#Paquete de chistes
import pyjokes 
#Paquete Time para delays
import time
#importa paquete para reproducir sonidos
import winsound
#importa la clase respuestas
from Responses import Responses
#Importa el servidor MQTT
from paho.mqtt import client as mqtt_client
#import random
import random
#import mqtt class
from mqtt import Mqtt 

global Mensaje
test=False

#Creación del listener
listener = sr.Recognizer()

#
engine = pyttsx3.init()

#Sets the voice to a female voice 0=SystemDefault 1=female
voices = engine.getProperty('voices')
engine.setProperty('voice',voices[1].id)

greeting="Hi Human, im your own AI , what can i do for you?"




#-----------------------------------------------Servidor MQTT----------------------------------
Mensaje=""
#broker = '192.168.1.2'
#port = 1883
topic = "Commands"
# generate client ID with pub prefix randomly
#client_id = f'python-mqtt-{random.randint(0, 100)}'





def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        Mensaje=str(msg.payload.decode("utf-8"))
        print(Mensaje)
        if Mensaje!="":
            run_ai(Mensaje)
            Mensaje=""
            client.disconnect()
    client.subscribe(topic)
    client.on_message = on_message
    client.loop_forever()
    


def run():
    client = Mqtt.connect_mqtt()
    subscribe(client)
    
   
       
    
        





#-----------------------------------------------Servidor MQTT----------------------------------

def talk(text):
    engine.say(text)
    engine.runAndWait()

#Descomenta para recuperar la funcionalidad original
#def wakeword():
    #try:
     #   with sr.Microphone() as source: 
      #    voice = listener.listen(source,phrase_time_limit=5)
       #   command =listener.recognize_google(voice)
        #  command = command.lower()
         # if 'alexa' in command:
          #    talk('What can i do for you?')
           #   ready=True
          #else:
           #     ready=False
    #except:
     #   ready=False
      #  pass
    #return ready 



#Metodo para acatar un comando
def take_command():
    try: 
        with sr.Microphone() as source: 
          winsound.PlaySound("listening",winsound.SND_FILENAME)
# Reconocimiento de voz 
          voice = listener.listen(source,phrase_time_limit=5)
          command =listener.recognize_google(voice)
          command = command.lower()
        
        if 'alexa' in command:
            # removes siri from command
            command=command.replace('alexa','')
            print(command) 
        
    except:
        command=""
        pass
    return command

# Metodo para correr comando
#El original no le pasas el mensaje
def run_ai(Mensaje):
    #Se usa el microfono de la computadora
    #command = take_command()
    command=Mensaje
    command = command.lower()
    print(command)
    if command!= "":
        if 'play' in command:
            song=command.replace('play','',1)
            Responses.playSong(song)

        elif 'weather' in command:
            Responses.Weather()

        elif 'time' in command:
            Responses.getTime()

        elif 'who is' in command:
            person=command.replace('who is','',1)
            Responses.searchPerson(person)


        elif 'search' in command:
            search=command.replace('search','',1)
            Responses.generalSearch(search)

        elif 'joke' in command:
           Responses.getJoke()

        elif 'what is' in command:
            info=command.replace('what is','',1)
            Responses.getInfo(info)

        elif 'call me' in command:
            newName=command.replace('call me','',1)
            Responses.setName(newName)

        elif 'tell me my name' in command:
            Responses.getName()

        elif 'how are you' in command:
            Responses.howAmI()

    else:
        talk("Sorry , i couldn't hear you, please try again")

  


#Run  Default Greeting
talk(greeting)

while True:
    run() #Elimina esta y linea y descomenta las demas para su funcionalidad original

#WakeWord
#while True:
    #Wake word Validation
    #startListening=wakeword()
    
#Funcion que me permita siempre estar escuchando
  #  if startListening == True:
        #Pide un comando
       # run_ai()
       # startListening = False
