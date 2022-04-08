// Sending test data over i2c, simulating databroker data
//--------------------------------------------------------------------

#include <Wire.h>
#include "JoyInput.h"
#include "GameState.h"
#include "DataHandler.h"

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

DataHandler dataHandler(0);

void setup() {
  // Safety pin. Connect 13 to make sure no mouse/keyboard messes up when reprogramming (if using Leonardo).
  //pinMode(13, INPUT_PULLUP);
  //while (digitalRead(13) == LOW) {  }

  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop() {
  //Transmit bits
  dataHandler.a = random(0,2);
  dataHandler.b = random(0,2);
  dataHandler.c = random(0,2);
  dataHandler.message[1] = random(0,9);
  dataHandler.message[2] = random(0,9);
  dataHandler.message[3] = random(0,9);

  dataHandler.transmit(SUB_CONTROLLER_POWER_DISTRIBUTION, 4);

  Serial.print(dataHandler.message[0]);
  Serial.print(dataHandler.message[1]);
  Serial.print(dataHandler.message[2]);
  Serial.println(dataHandler.message[3]);

  //Request bytes
  Wire.requestFrom(10, 2);    // request bytes from slave device
  while (Wire.available()) { // slave may send less than requested
    Serial.print(Wire.read()); // receive a byte as character
    Serial.print(" ");
  }
  Serial.println();

  delay(1000);
}



/* OLD VERSION
void gameStateFromNodeRED() {
  if (Serial.available() > 0) {
    bool readyToInterpret = false;
    while (Serial.available() > 0) {
      data[dataCurrentPosition] = Serial.read();
      if (data[dataCurrentPosition] == '{' && dataCurrentPosition != 0) {
        dataCurrentPosition = 0;
        data[0] = '{';
      }

      if (data[dataCurrentPosition] == '}') {
        dataCurrentPosition = 0;
        readyToInterpret = true;
        break;
      }
      if (data[dataCurrentPosition] == ',') {
        dataCurrentPosition = 0;
        readyToInterpret = true;
        break;
      }    
      dataCurrentPosition++;
      if (dataCurrentPosition > 20) {
        dataCurrentPosition = 0;
      }
    }

    //  char data[20] = "{\"firegroup\":2}";
    //  if(1){
    if (readyToInterpret) {
      char key[20] = "";
      char val[20] = "";
      int keyCurrentPosition = 0;
      int valCurrentPosition = 0;
      int breakPlacement = 0;
      
      for (int i = 1; i < arr_len(data); i++) {
        if (data[i] == ':') {
          breakPlacement = i;
          break;
        }
      }
      if (breakPlacement != 0) {
        for (int i = 0; i < breakPlacement; i++) {
          if(data[i] != '{' && data[i] != ' ' && data[i] != ','){
            key[keyCurrentPosition] = data[i + 2]; 
            keyCurrentPosition++;
          }
        }
        for (int i = breakPlacement; i < arr_len(data); i++) {
          if(data[i] == '}' || data[i] == ','){ 
            break;
          }
          if(data[i] != '{' && data[i] != ' ' && data[i]){
            val[valCurrentPosition] = data[i];
            valCurrentPosition++;
          }
        }
        gameState.interpretSerialData(key, val);      // Function in Game State
      }
    }
  }
}
*/

/*  Transmit Data, example code.


  //Transmit data to sub-controllers
  Wire.beginTransmission(9); // transmit to device #8
  Wire.write(c);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(c);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  x++;
  if(x>15){ x = 0; }

  Wire.requestFrom(8, 1);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    c = Wire.read(); // receive a byte as character
  }
*/
