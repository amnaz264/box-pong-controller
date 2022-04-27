//===================================================
// mqtt LISTEN function
void mqttMessageReceive(int messageSize){

  // we received a message, print out the topic and contents

  String theTopic = mqttClient.messageTopic();
  // debug message details
    // Serial.print("Received a message with topic ::");
    // Serial.println(theTopic);
    // Serial.print("and length ");
    // Serial.print(messageSize);
    // Serial.println(" bytes:");


  // extract data -- game always sends with following structure
  // [  * messageType , whichPlayer=1/2 ,# ]
  //  where messageType=HIT/GOAL/WINNER/RESET
  //  where whichPlayer == 1/2

  if (cut.listen() > 0) {
    messageType = cut.getInt(0);  // message kind - HIT, GOAL, WINNER
    whichPlayer = cut.getInt(1);  // which player -- 1,2

    // debug the incoming playload
      Serial.print("in <-- :: ");
      Serial.println(cut.getRaw());
  } // end if

  //**************************************************
  // A player has hit the ball
  if (messageType == HIT){
    Serial.print("hit ");  Serial.println(whichPlayer);
    // determine IF i did that
    if ( whichPlayer == MY_PLAYER_INDEX ){
      Serial.println("that's me ");
      // add a controller action for my HITs here
      // ...
      blinkLed (100, 20, 0, 500, 250, 3);

    } else {
      // add a controller action for opponent HITs here
      // ...
      blinkLed (100, 0, 0, 250, 250, 3);
    }
  }

  //**************************************************
  // a player scored a goal
  if (messageType == GOAL){
    // player index will hold the player that scored
    Serial.print("GOAL! scored by ");  Serial.println(whichPlayer);

    if ( whichPlayer == MY_PLAYER_INDEX ){
      Serial.println("that's me ");
      // add a controller action for my GOALs here
      // ...
      blinkLed (40, 20, 110, 500, 250, 3);

    } else {
      Serial.println("not me");
      // add a controller action for opponent GOALs here
      // ...
      blinkLed (100, 0, 0, 250, 250, 3);
    }
  }

  //**************************************************
  // a player has won the game
  if (messageType == WINNER ){
    // player index will hold the player that scored
    Serial.print("player ");  Serial.print(whichPlayer);  Serial.println(" wins");

    if ( whichPlayer == MY_PLAYER_INDEX ){
      Serial.println("that's me ");

      // add a controller action for my WIN here
      // ...
      blinkLed (50, 100, 100, 250, 500, 3);
        blinkLed (50, 110, 120, 250, 500, 3);

    } else {
      Serial.println("not me");
      // add a controller action for my LOSS here
      // ...
      blinkLed (110, 0, 0, 500, 500, 3);
    }
  }

  //**************************************************
  // a player has won the game
  if (messageType == RESET ){
    // player index will hold the player that scored
    Serial.print(" ****  game reset **** ");  Serial.println("\n\n\n");

    // add controller action for reset HERE if desired


  }

} // end mqttMessageReceive()


void blinkLed( int _r, int _g, int _b, int _on, int _off, int _howMany){
  for ( int i = 0 ; i < _howMany; i++) {
    setColor(_r, _g, _b);
    delay(_on);
    ledOff();
    delay(_off);

  }
}

void setColor( int _r, int _g, int _b) {
  analogWrite(redPin, _r);
  analogWrite(grnPin, _g);
  analogWrite(bluPin, _b);
}

void ledOff() {
  analogWrite(redPin, 0);
  analogWrite(grnPin, 0);
  analogWrite(bluPin, 0);
}
