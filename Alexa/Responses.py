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
#Paquete numero random
import random
#Importa el usuario
from User import User
#Importa el servidor mqtt
from mqtt import Mqtt 
#Importa el servidor MQTT
from paho.mqtt import client as mqtt_client

#Creación del listener
listener = sr.Recognizer()

#
engine = pyttsx3.init()

#Sets the voice to a female voice 0=SystemDefault 1=female
voices = engine.getProperty('voices')
engine.setProperty('voice',voices[1].id)

#---------------MQTT------------

topic = "Weather"




def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        Clima=str(msg.payload.decode("utf-8"))
        if Clima!="":
                talk("The Weather today is: "+Clima+"°")
                print("The Weather today is: "+Clima+"°")
                client.disconnect()
        else:
                talk("Sorry i couldn´t get the weather")
                client.disconnect()
    client.subscribe(topic)
    client.on_message = on_message  
    client.loop_forever()       
            
#---------------MQTT------------

def talk(text):
    engine.say(text)
    engine.runAndWait()



class Responses:
    #Metodos con todas las respuestas
    def playSong(song):
         if song!="":
                talk('Playing '+song)
                pywhatkit.playonyt (song)
         else:
                talk("Sorry , i couldn't hear you, please try again")

    def getTime():
            time= datetime.datetime.now().strftime('%I:%M %p')
            print(time)
            talk('The current time is '+time)

    def searchPerson(person):
        if person!="":
            try:
                info=wikipedia.summary(person,1,auto_suggest=False)
                print(info)
                talk(info)
                #in case it finds more than 1 definition ,takes a random one
            except wikipedia.DisambiguationError as e:
                s = random.choice(e.options)
                p=wikipedia.summary(s,1,auto_suggest=False)
                talk(p)
            except wikipedia.exceptions.PageError as e:
                talk("Sorry , i couldn't find that page")    
        else:
                talk("Sorry , i couldn't hear you, please try again")

    def generalSearch(search):
        if search!="":
                talk('Searching '+search)
                pywhatkit.search(search)
        else:
                talk("Sorry , i couldn't hear you, please try again")

    def getJoke():
            joke=pyjokes.get_joke()
            print(joke)
            talk(joke)

    def getInfo(info):
        if info!="":
            try:
                summary=wikipedia.summary(info,1,auto_suggest=False)
                print(summary)
                talk(summary)
                #in case it finds more than 1 definition ,takes a random one
            except wikipedia.DisambiguationError as e:
                s = random.choice(e.options)
                p=wikipedia.summary(s,1,auto_suggest=False)
                talk(p)
            except wikipedia.exceptions.PageError as e:
               talk("Sorry , i couldn't find that page")   
        else:
                talk("Sorry , i couldn't hear you, please try again")

    def setName(newName):
        if newName!="":
            talk("OK,From now on i'll call you "+str(newName))
            User.setName(User,newName)
        else:
                talk("Sorry , i couldn't hear you, please try again")

    def getName():
           name= User.getName(User)
           talk("Your name is "+str(name))

    def howAmI():
         name= User.getName(User)
         feelings = random.randint(1, 2)
         if feelings== 1:
             talk("Im fine "+str(name)+", Thank you for asking")
         if feelings== 2:
             talk("Im not that fine "+str(name))

    def Weather():
        client = Mqtt.connect_mqtt2()
        subscribe(client)
        
        
        
    

            

    
         
            