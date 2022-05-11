#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "mrsh77";
const char* password = "1m77n2299215r77#";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.0.101";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);
/*
int seeupBtn = 2;
int upState;

int seedownBtn = 12;
int downState;

int seerightBtn = 13;
int rightState;

int seeleftBtn = 15;
int leftState;
*/
Adafruit_MPU6050 mpu;

int Xacceleration;
int Yacceleration;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");

}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  /*
  pinMode(seeupBtn,INPUT);
  pinMode(seedownBtn,INPUT);
  pinMode(seerightBtn,INPUT);
  pinMode(seeleftBtn,INPUT);
  */
/*  
  upState = 0;
  downState = 0;
  rightState = 0;
  leftState = 0;
*/
  Serial.begin(115200);
  Serial.setTimeout(1);
  
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

    Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
 
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {
/*
  upState = digitalRead(seeupBtn);
  downState = digitalRead(seedownBtn);
  rightState = digitalRead(seerightBtn);
  leftState = digitalRead(seeleftBtn);
*/
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  
  Xacceleration = a.acceleration.x;
  Yacceleration = a.acceleration.y;

/*
  Serial.print("X= ");
  Serial.println(Xacceleration);
  Serial.print("Y= ");
  Serial.println(Yacceleration);
  Serial.println("---------------------");
  delay(1000);
*/

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");

    if((Xacceleration > 7) && (Yacceleration < 3)){
      client.publish("cmd","b");
      //Serial.println("b");
      //delay(1000);
    }
    else if((Yacceleration < 3) && (Xacceleration < -7)){
      client.publish("cmd","f");
      //Serial.println("f");
      //delay(1000);
    }
    else if((Xacceleration < 3) && (Yacceleration < -6)){
      client.publish("cmd","l");
      //Serial.println("l");
      //delay(1000);
    }
    else if((Xacceleration < 3) && (Yacceleration > 6)){
      client.publish("cmd","r");
      //Serial.println("r");
      //delay(1000);
    }
/*
    else if(upState == HIGH){
      Serial.println("see UP");
      //client.publish("cmd","w");
      delay(500);
    }
    else if(downState == HIGH){
      Serial.println("see DOWN");
      //client.publish("cmd","p");
      delay(500);
    }
    else if(rightState == HIGH){
      Serial.println("see RIGHT");
      //client.publish("cmd","d");
      delay(500);
    }
    else if(leftState == HIGH){
      Serial.println("see LEFT");
      //client.publish("cmd","a");
      delay(500);
    }
*/
    else{
      //Serial.println("s");
      //delay(1000);
      client.publish("cmd","s");
    }
} 
