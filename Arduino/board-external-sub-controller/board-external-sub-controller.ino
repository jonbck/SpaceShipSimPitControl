#include <Wire.h>
#include "SubController.h"
#include "DataHandler.h"

SubController inDangerSignal(JOY_DATA_LED, 11);
SubController heatDamageSignal(JOY_DATA_LED, 10);
SubController errorSolving(JOY_DATA_LED, 0); // For some reason the last one doesn't get its pin. Need to solve better.

DataHandler dataHandler(12);
long timestamp;
long triggerTime = 0;

const long fireLength = 1000;					// 1 sek
const long fireCooldownLength = 10000;	// 10 sek

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(SUB_CONTROLLER_BOARD_EXTERNAL);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
	timestamp = millis();
    
  //Read i2c (happens in interrupts)
	
  //Update accordingly
  inDangerSignal.animateLight();
  heatDamageSignal.animateLight();

  //Read buttons
  
  //Send i2c-data

  delay(10);
}

void receiveEvent(int bytesReceived) {
  dataHandler.receiveEvent(bytesReceived);

  if(dataHandler.a){
    inDangerSignal.setValue(true);
  }else{
    inDangerSignal.setValue(false);
  }
  if(dataHandler.b){
    if(timestamp > triggerTime + fireLength + fireCooldownLength){
      //In pause mode. Ready to start new fire
      heatDamageSignal.setValue(true);
      triggerTime = timestamp;
    }else if(timestamp > triggerTime && timestamp < triggerTime + fireLength){
      //In ongoing fire. Ready to restart current
      heatDamageSignal.setValue(true);
    }else if(timestamp > triggerTime + fireLength){
      //In cooldown
      heatDamageSignal.setValue(false);
    }else{
      //Timestamp is below. Shoulden't normally happen. Reset all.
      heatDamageSignal.setValue(false);
      triggerTime = timestamp;
    }
  }else{
    heatDamageSignal.setValue(false);
  }
}

void requestEvent() {
  dataHandler.replyEvent(1);
}
