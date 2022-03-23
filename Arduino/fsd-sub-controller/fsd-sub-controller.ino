#include <Wire.h>
#include "SubController.h"
#include "DataHandler.h"

SubController coolDownLight(JOY_DATA_LED, 6);
SubController massLockedLight(JOY_DATA_LED, 7);
SubController chargeLight(JOY_DATA_LED, 8);
SubController fsdLight(JOY_DATA_LED, 9);
SubController superCruiseLight(JOY_DATA_LED, 10);
SubController nextInRouteLight(JOY_DATA_LED, 11);
SubController hyperSpaceLight(JOY_DATA_LED, 12);
SubController fsdButton(JOY_BUTTON, 2);
SubController superCruiseButton(JOY_BUTTON, 3);
SubController nextInRouteButton(JOY_BUTTON, 4);
SubController hyperSpaceButton(JOY_BUTTON, 5);
DataHandler dataHandler(12);

void setup() {
  Wire.begin(SUB_CONTROLLER_FSD);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
  //Read i2c (happens in interrupts)
  //Update accordingly
  coolDownLight.animateLight();
  massLockedLight.animateLight();
  chargeLight.animateLight();
  superCruiseLight.animateLight();
  hyperSpaceLight.animateLight();
  fsdLight.animateLight();

  //Read buttons
  fsdButton.readValue();
  superCruiseButton.readValue();
  nextInRouteButton.readValue();
  hyperSpaceButton.readValue();
  
  //Send i2c-data

  /* Read switches (example)
  switches = 0;
  if(digitalRead(switch1) == LOW){
    switches = switches | B00000001;
  }
  if(digitalRead(switch2) == LOW){
    switches = switches | B00000010;
  }

  //Set lights
  if(received & B00000001){
    digitalWrite(light1, LOW); 
  }else{
    digitalWrite(light1, HIGH);     
  }
  */

  delay(10);
}

void receiveEvent(int bytesReceived) {
  dataHandler.receiveEvent(bytesReceived);

  if(dataHandler.c){
    chargeLight.setValue(true);
  }else{
    chargeLight.setValue(false);
  }
  if(dataHandler.a){
    coolDownLight.setValue(true);
  }else{
    coolDownLight.setValue(false);
  }
  if(dataHandler.b){
    massLockedLight.setValue(true);
  }else{
    massLockedLight.setValue(false);
  }
  if(dataHandler.d){
    superCruiseLight.setValue(true);
  }else{
    superCruiseLight.setValue(false);
  }
  if(dataHandler.e){
    hyperSpaceLight.setValue(true);
  }else{
    hyperSpaceLight.setValue(false);
  }
  
  if(dataHandler.a || dataHandler.b || dataHandler.c){
        fsdLight.setValue(JOY_LED_BLINK);
  }else if(dataHandler.d || dataHandler.e){
     fsdLight.setValue(JOY_LED_ON);
  }else{
    fsdLight.setValue(JOY_LED_OFF);
  }
}

void requestEvent() {
  dataHandler.a = fsdButton.getValue();
  dataHandler.b = superCruiseButton.getValue();
  dataHandler.c = nextInRouteButton.getValue();
  dataHandler.d = hyperSpaceButton.getValue();
  Serial.print(dataHandler.a);
  Serial.print(dataHandler.b);
  Serial.print(dataHandler.c);
  Serial.println(dataHandler.d);
  dataHandler.replyEvent(1);
}
