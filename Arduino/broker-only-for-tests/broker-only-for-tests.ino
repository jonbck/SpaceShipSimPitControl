// Sending test data over i2c, simulating databroker data
//--------------------------------------------------------------------

#include <Wire.h>
#include "JoyInput.h"
#include "GameState.h"
#include "DataHandler.h"

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

DataHandler dataHandler(1);

bool sent = false;

void setup() {
  // Safety pin. Connect 13 to make sure no mouse/keyboard messes up when reprogramming (if using Leonardo).
  //pinMode(13, INPUT_PULLUP);
  //while (digitalRead(13) == LOW) {  }
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop() {
  //Transmit bits

  dataHandler.clearData();
  dataHandler.a = sent;
  dataHandler.b = sent;

  dataHandler.transmit(SUB_CONTROLLER_BOARD_EXTERNAL, 1);

//  Serial.print(dataHandler.a);
//  Serial.println(dataHandler.b);
  digitalWrite(LED_BUILTIN, dataHandler.a);

  if(sent){
    sent = false;
  }else{
    sent = true;
  }
  
  delay(1000);
}
