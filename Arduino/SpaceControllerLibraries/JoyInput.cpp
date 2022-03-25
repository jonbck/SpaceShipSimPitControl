#include "JoyInput.h"

JoyInput::JoyInput(int type, int pin, int low = 0, int midlow = 0, int midhigh = 0, int high = 0){
  this->type = type;
  this->pin = pin;
  this->current = 0;

  if(this->type == JOY_STICK){
    this->low = low;
    this->midlow = midlow;
    this->midhigh = midhigh;
    this->high = high;
  }

  if(this->type == JOY_BUTTON){
    this->inputHasChanged = false;
    pinMode(this->pin, INPUT_PULLUP); 
  }
  
}
void JoyInput::getPin(){
  return this->pin;
}
int JoyInput::getValue(){
  return this->current;
}
bool JoyInput::hasChanged(){
  if(this->inputHasChanged){
    this->inputHasChanged = false;
    return true;
  }else{
    return false;
  }
}
void JoyInput::readValue(){
  if(this->type == JOY_STICK){
    int sensorValue;
    sensorValue = analogRead(this->pin);
    this->sensorvalue = sensorValue;
    if(sensorValue <= this->midlow){
      sensorValue = map(sensorValue, this->low, this->midlow, 0, 127);
    }else if(sensorValue <= this->midhigh){
      sensorValue = 128;
    }else{
      sensorValue = map(sensorValue, this->midhigh, this->high, 128, 255);
    }
    if(sensorValue > 255){ sensorValue = 255; }
    if(sensorValue < 0){ sensorValue = 0; }
    this->current = sensorValue;
  }else if(this->type == JOY_BUTTON){
    bool sensorValue;
    sensorValue = !digitalRead(this->pin);
    // Add different assignment according to button type (swich/momentary/moment2switch etc)
    if(sensorValue != this->current){
      this->current = sensorValue;
      this->inputHasChanged = true;
    }
  }
    /*        
    Serial.print(this->pin);
    Serial.print(": ");
    Serial.print(this->sensorvalue);
    Serial.print(" (");
    Serial.print(this->current);
    Serial.print(" ) - ");
    */
}
