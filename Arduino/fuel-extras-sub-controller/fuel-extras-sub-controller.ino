#include <Wire.h>
#include "SubController.h"
#include "DataHandler.h"

//Shift register values
const int dataPin = 2;
const int latchPin = 3;
const int clockPin = 4;
byte shiftRegLeds[] = {0,0,0};
// 3 bytes to shift register as below:
// shiftRegLeds[0], 0 - 7  --> fuel gauge from low (red) to high (green)
// shiftRegLeds[1], 0      --> fuel gauge high (green)
// shiftRegLeds[1], 1      --> Dock
// shiftRegLeds[1], 2      --> Shield
// shiftRegLeds[1], 3      --> Fuel Scoop
// shiftRegLeds[1], 4      --> Flight Assist Off
// shiftRegLeds[1], 5      --> Silent Running
// shiftRegLeds[1], 6      --> Cargo Scoop
// shiftRegLeds[1], 7      --> Night Vision
// shiftRegLeds[2], 0      --> Orbit Lines
// shiftRegLeds[2], 1      --> Landing Gear
// shiftRegLeds[2], 2      --> Ship Light
// shiftRegLeds[2], 3      --> Half Lights
// shiftRegLeds[2], 4 - 7  --> N/A

SubController fuelGaugeMeter(JOY_DATA);
SubController dockLight(JOY_DATA);
SubController shieldLight(JOY_DATA);
SubController fuelScoopLight(JOY_DATA);
SubController flightAssistOffLight(JOY_DATA);
SubController silentRunningLight(JOY_DATA);
SubController cargoScoopLight(JOY_DATA);
SubController nightVisionLight(JOY_DATA);
SubController orbitLinesLight(JOY_DATA);
SubController landingGearLight(JOY_DATA);
SubController shipLightsLight(JOY_DATA);
SubController halfLightsLight(JOY_DATA);

SubController silentRunningSwitch(JOY_BUTTON, 6);
SubController flightAssistOffSwitch(JOY_BUTTON, 7);
SubController halfLights(JOY_BUTTON, 8);
SubController shipLights(JOY_BUTTON, 9);
SubController landingGear(JOY_BUTTON, 10);
SubController orbitLines(JOY_BUTTON, 11);
SubController nightVision(JOY_BUTTON, 12);
SubController cargoScoop(JOY_BUTTON, 13);
DataHandler dataHandler(12);

void setup() {
  Wire.begin(SUB_CONTROLLER_FUEL);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
 
  //Set up shiftregister out
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
}

void loop() {
    //Read i2c (happens in interrupts)
  //Update accordingly
  // (Everything in shift register, special lights below)

  //Animate special lights

  //Set Fuel Gauge
  int fuel = map(fuelGaugeMeter.getValue(), 0, 255, 0, 9);

  bitClear(shiftRegLeds[1],0);  // Set bit 9 to 0

  if(fuel == 9){
    shiftRegLeds[0] = 255;
    bitSet(shiftRegLeds[1],0);  // Set bit 9 to 1
  }else if(fuel == 8){
    shiftRegLeds[0] = 255;
  }else if(fuel == 7){
    shiftRegLeds[0] = 127;
  }else if(fuel == 6){
    shiftRegLeds[0] = 63;
  }else if(fuel == 5){
    shiftRegLeds[0] = 31;
  }else if(fuel == 4){
    shiftRegLeds[0] = 15;
  }else if(fuel == 3){
    shiftRegLeds[0] = 7;
  }else if(fuel == 2){
    shiftRegLeds[0] = 3;
  }else if(fuel == 1){
    shiftRegLeds[0] = 1;
  }else{
    shiftRegLeds[0] = 0;
  }

  //Set button lights
  if(dockLight.getValue()){
    bitSet(shiftRegLeds[1],1);
  }else{
    bitClear(shiftRegLeds[1],1);
  }

  if(shieldLight.getValue()){
    bitSet(shiftRegLeds[1],2);
  }else{
    bitClear(shiftRegLeds[1],2);
  }

  if(fuelScoopLight.getValue()){
    bitSet(shiftRegLeds[1],3);
  }else{
    bitClear(shiftRegLeds[1],3);
  }

  if(flightAssistOffLight.getValue()){
    bitSet(shiftRegLeds[1],4);
  }else{
    bitClear(shiftRegLeds[1],4);
  }

  if(silentRunningLight.getValue()){
    bitSet(shiftRegLeds[1],5);
  }else{
    bitClear(shiftRegLeds[1],5);
  }

  if(cargoScoopLight.getValue()){
    bitSet(shiftRegLeds[1],6);
  }else{
    bitClear(shiftRegLeds[1],6);
  }

  if(nightVisionLight.getValue()){
    bitSet(shiftRegLeds[1],7);
  }else{
    bitClear(shiftRegLeds[1],7);
  }

  if(orbitLinesLight.getValue()){
    bitSet(shiftRegLeds[2],0);
  }else{
    bitClear(shiftRegLeds[2],0);
  }

  if(landingGearLight.getValue()){
    bitSet(shiftRegLeds[2],1);
  }else{
    bitClear(shiftRegLeds[2],1);
  }

  if(shipLightsLight.getValue()){
    bitSet(shiftRegLeds[2],2);
  }else{
    bitClear(shiftRegLeds[2],2);
  }

  if(halfLightsLight.getValue()){
    bitSet(shiftRegLeds[2],3);
  }else{
    bitClear(shiftRegLeds[2],3);
  }

  //Update lights
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, shiftRegLeds[2]);
  shiftOut(dataPin, clockPin, MSBFIRST, shiftRegLeds[1]);
  shiftOut(dataPin, clockPin, MSBFIRST, shiftRegLeds[0]);
  digitalWrite(latchPin, HIGH);

  //Read buttons
  silentRunningSwitch.readValue();
  flightAssistOffSwitch.readValue();
  halfLights.readValue();
  shipLights.readValue();
  landingGear.readValue();
  orbitLines.readValue();
  nightVision.readValue();
  cargoScoop.readValue();
  
  delay(10);
}

void receiveEvent(int bytesReceived) {
  dataHandler.receiveEvent(bytesReceived);

  fuelGaugeMeter.setValue(dataHandler.message[2]);
  dockLight.setValue(dataHandler.a);
  shieldLight.setValue(dataHandler.b);
  fuelScoopLight.setValue(dataHandler.c);
  flightAssistOffLight.setValue(dataHandler.d);
  silentRunningLight.setValue(dataHandler.e);
  cargoScoopLight.setValue(dataHandler.f);
  nightVisionLight.setValue(dataHandler.g);
  orbitLinesLight.setValue(dataHandler.h);
  landingGearLight.setValue(bitRead(dataHandler.message[1], 0));
  shipLightsLight.setValue(bitRead(dataHandler.message[1], 1));
  halfLightsLight.setValue(bitRead(dataHandler.message[1], 2));

}

void requestEvent() {
  dataHandler.a = silentRunningSwitch.getValue();
  dataHandler.b = flightAssistOffSwitch.getValue();
  dataHandler.c = halfLights.getValue();
  dataHandler.d = shipLights.getValue();
  dataHandler.e = landingGear.getValue();
  dataHandler.f = orbitLines.getValue();
  dataHandler.g = nightVision.getValue();
  dataHandler.h = cargoScoop.getValue();

  Serial.print(dataHandler.a);
  Serial.print(dataHandler.b);
  Serial.print(dataHandler.c);
  Serial.print(dataHandler.d);
  Serial.print(dataHandler.e);
  Serial.print(dataHandler.f);
  Serial.print(dataHandler.g);
  Serial.println(dataHandler.h);

  dataHandler.replyEvent(1);
}