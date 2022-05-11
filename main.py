import paho.mqtt.client as mqtt
from picarx import Picarx
from robot_hat import TTS
from multiprocessing import Process
import cv2


ttsBot = TTS()

px = Picarx()


MQTT_SERVER = "localhost" #specify the broker address, it can be IP of raspberry pi or simply localhost
MQTT_PATH = "cmd"

def camfeed():
    cap = cv2.VideoCapture(0)   
    while True:
        success, img = cap.read()

        cv2.imshow("Image", img)
        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break

def sayObstacle():

    while True:

        distance = px.ultrasonic.read()
        #print(distance)
        if distance <5:
            ttsBot.say("there is obstacle front of me")
        

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    client.subscribe(MQTT_PATH)

def on_message(client, userdata, message):
    direction = message.payload.decode().strip()
    #print("Received message '" + str(message.payload) + "' on topic '" + message.topic)
    #print(direction)
    if direction == "s":
        px.forward(0)
        px.set_dir_servo_angle(0)
        
    elif direction == "f":
        px.set_dir_servo_angle(0)
        px.forward(1)
        
    elif direction == "b":
        px.set_dir_servo_angle(0)
        px.forward(-1)
        
    elif direction == "r":
        px.set_dir_servo_angle(45)
        px.forward(1)
        
    elif direction == "l":
        px.set_dir_servo_angle(-45)
        px.forward(1)
        
def main():
    mqtt_client = mqtt.Client()
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_SERVER, 1883, 60) 
    # Connect to the MQTT server and process messages in a background thread. 
    mqtt_client.loop_start() 

if __name__ == '__main__':

    main()
    
    p1 = Process(target=camfeed)
    p2 = Process(target=sayObstacle)
    
    p1.start()
    p2.start()
