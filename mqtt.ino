// note mqttClient is globally defined in main tab

//===================================================
// mqtt initialization (assumes connecting to shiftr)
void initMqtt(){
 // https://github.com/arduino-libraries/ArduinoMqttClient/blob/master/examples/WiFiSimpleSender/WiFiSimpleSender.ino

  mqttClient.setId(id); // device name
  mqttClient.setUsernamePassword(user, mqttPW);

  Serial.print("\nconnecting to shiftr...");
  while (!mqttClient.connect(broker,port)) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\nconnected to shiftr @");
  Serial.println(user); // instance name
} // end initMqtt

//===================================================
// mqtt SEND function
void publishMqttMessage(String topicString, String payload){
   mqttClient.beginMessage(topicString);
   mqttClient.print(payload);
   mqttClient.endMessage();

   // Serial.print("out --> :: ");
   //  Serial.print(topicString);
   //   Serial.print("  ");
   //     Serial.println(payload);

}
