#include "SubController.h"

/* ********** ToDo: **********
 
Add support for latching buttons
Fix lights for shift register leds
Fix animation for shift register controlled stack of lights

********** ********** */

SubController::SubController(int type, int pin = false, int a = false, int b = false, int c = false, int d = false){
  this->type = type;
  this->pin = pin;
  this->current = 0;
  this->timestamp = 0;
  this->animationTime = 0;

  if(this->type == JOY_STICK){
    this->low = a;
    this->midlow = b;
    this->midhigh = c;
    this->high = d;
  }

  if(this->type == JOY_MULTIBUTTON){
    this->low = a;
    this->high = b;
  }

  if(this->type == JOY_MULTIBUTTON){
    this->inputHasChanged = false;
    pinMode(this->pin, INPUT);
  }

  if(this->type == JOY_BUTTON){
    this->inputHasChanged = false;
    pinMode(this->pin, INPUT_PULLUP); 
  }

  if(this->type == JOY_DATA_LED){
    pinMode(this->pin, OUTPUT);
    this->ledMode = JOY_LED_OFF;
  }
}

int SubController::getPin(){
  return this->pin;
}

int SubController::getValue(){
  return this->current;
}

void SubController::setLight(){
  digitalWrite(this->pin, this->current);
}

void SubController::setValue(int newValue){
  if(this->current != newValue){
    this->current = newValue;
    this->inputHasChanged = true;
  }
}

bool SubController::hasChanged(){
  if(this->inputHasChanged){
    this->inputHasChanged = false;
    return true;
  }else{
    return false;
  }
}
void SubController::animateLight(){
  if(this->current == JOY_LED_BLINK){
    int deltaTime = millis() - this->timestamp;
    this->timestamp = this->timestamp + deltaTime;
    this->animationTime = this->animationTime + deltaTime;
    if(this->animationTime < 1000){
      digitalWrite(this->pin, HIGH);
    }else if(this->animationTime < 2000){
      digitalWrite(this->pin, LOW);
    }else{
      this->animationTime = 0;
    }
  }else{
    if(hasChanged()){
      setLight();
    }
  }
}

void SubController::readValue(){
  if(this->type == JOY_BUTTON){
    bool sensorValue;
    sensorValue = !digitalRead(this->pin);
    // Add different assignment according to button type (swich/momentary/moment2switch etc)
    if(sensorValue != this->current){
      this->current = sensorValue;
      this->inputHasChanged = true;
    }
  }else if(this->type == JOY_MULTIBUTTON){
    int sensorValue;
    bool newValue;
    sensorValue = analogRead(this->pin);
    if(sensorValue >= this->low && sensorValue <= this->high){
      newValue = true;
    }else{
      newValue = false;
    }
    if(newValue != this->current){
      this->current = newValue;
      this->inputHasChanged = true;
    }
  }else if(this->type == JOY_STICK){
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
  }
}
