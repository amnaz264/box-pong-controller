//===================================================
// network library includes
#include <WiFi.h>
#include <ArduinoMqttClient.h> // https://github.com/arduino-libraries/ArduinoMqttClient

// wifi log on credentials -- find in secrets.h tab
#include "secrets.h"

//===================================================
// global variable for network -- DO NOT CHANGE
// wifi and mqtt clients
WiFiClient wifiClient;  // part of wifi.h
MqttClient mqttClient(wifiClient);


//***************************************************
// PLAYER specific variables -- you need to modify
// on game day your <pongPlayerTopic> will be:
//  playerOne
//  or
//  playerTwo

String pongPlayerTopic  = "playerOne-az";
int MY_PLAYER_INDEX = 1; //set to 1 if playerOne, 2 if playerTwo
String playerName = "amna"; // your player name



//===================================================
// MQTT message topics and variables - DO NOT CHANGE

String sendMessage= "";
String subscribeTopic = "pongGame";
int messageType, whichPlayer;

// codes for message FROM the game (feedback)
#define HIT 1
#define GOAL 2
#define WINNER 3
#define RESET 4


//===================================================
// GLUE and SCISSORS libraries support message protocol
#include <Glue.h>
Glue elmers;

#include <Scissors.h>
Scissors cut;


//**************************************************
    // global circuit variables -- project specific
    // variabels for pins and sensor values
    int paddlePosition = 127; // default = middle of screen

    // your code here

    int touchPin1 = T9;
    int touchPin2 = T8;
    int touchValue1;
    int touchValue2;

    int touchState1, touchState2;

    int old_touchState1;
    int old_touchState2;

    int threshold = 40;

    int redPin = 22;
    int grnPin = 23;
    int bluPin = 14;



//*********

//==++==++==++==++==++==++==++==++==++==++==++==++==
// send timer -- we should listen more than we send
unsigned long sendInterval = 50; //==++ --> affects performance
unsigned long sendStartTime = 0;
unsigned long currentTime = 0;



//===================================================
void setup() {
  Serial.begin(9600);
  elmers.begin();  // not attaching it to a stream

  initWiFi();

if ( WiFi.status() == WL_CONNECTED)  { // if wifi connects attempt to connect to shiftr
    initMqtt();
  }

  // subscribe to game topic
  mqttClient.subscribe(subscribeTopic);
  // set the message receive callback
  mqttClient.onMessage(mqttMessageReceive);
  // create and start the payload parser
  cut.begin(mqttClient);


  //*********** CIRCUIT specific variables ***********

      // your code here ...





      pinMode (redPin, OUTPUT);
      pinMode (grnPin, OUTPUT);
      pinMode (bluPin, OUTPUT);


  //**************************************************

} // end setup

//===================================================
void loop(){
  currentTime = millis();
  mqttClient.poll(); // need to keep Mqtt connection alive

  if ((currentTime - sendStartTime) > sendInterval){
    //**************************************************
    // guts of your controller here
    // your code here
    // or
    sensorRead();
    calculatePosition();

    buildPongMessage(paddlePosition);
    //**************************************************
    sendStartTime = currentTime;
    //debugSensors();
  }
} // end loop

//**************************************************
// contents of this function are circuit dependant
void sensorRead(){

   // your code here
   //Serial.println("readSensors");
   touchState1 = 0;
   touchState2 = 0;

  touchValue1 = touchRead(touchPin1);
  if ( touchValue1 <= threshold) touchState1 = 1;

  touchValue2 = touchRead(touchPin2);
  if ( touchValue2 <= threshold) touchState2 = 1;

}

//**************************************************
// you will likely need to do some work with the raw sensor reading
// to get your paddle position
void calculatePosition(){

   // your code here
   // prevent repeats
     // if (touchState1 == 1 ){
     //   if ( old_touchState1 != touchState1 )  myPosition -= 10;
     // }
     // if (touchState2 == 1 ){
     //   if ( old_touchState2 != touchState2)  myPosition += 10;
     // }
     // allow repeats
       if (touchState1 == 1 ){
       paddlePosition -= 10;
     }
     if (touchState2 == 1 ){
       paddlePosition += 10;
     }
     // keep on screen
     if ( paddlePosition <= 0) paddlePosition = 0;
     if ( paddlePosition >= 255) paddlePosition = 255;

     old_touchState1 = touchState1;
     old_touchState2 = touchState2;
 }

//**************************************************
// build your controller message of form
// [ * playerName , playerMove , #]
void buildPongMessage(int moveValue){
  // use elmers to add the parsing syntax around value -- [*,#]
  elmers.create();
  elmers.add("amna");
  elmers.add(moveValue);
  elmers.endPackage();
  String payload = elmers.getPackage();

  publishMqttMessage(pongPlayerTopic, payload);
}

//**************************************************
void debugSensors(){
  // contents here should reflect your sensors
  // decide if you are plotting or monitoring

  // your code here

}
