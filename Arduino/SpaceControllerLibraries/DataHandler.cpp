#include "DataHandler.h"
#include <Wire.h>

//Test

DataHandler::DataHandler(int example){
  this->example = example;
}

int DataHandler::setExample(int example){
  this->example = example;
  return this->example;
}

void DataHandler::receiveEvent(int bytesReceived) {
  if(bytesReceived == 1){
    // Only one byte. Data coded in bits.
    byte received = Wire.read();
    this->a = received & 1;
    this->b = received & 2;
    this->c = received & 4;
    this->d = received & 8;
    this->e = received & 16;
    this->f = received & 32;
    this->g = received & 64;
    this->h = received & 128;
  }else{
    // Several bytes (probably 2). Decode as key + val
    while (Wire.available() > 0) {
      Serial.print(Wire.read());
    }
    Serial.println("[END]");
  }
}

void DataHandler::replyEvent(int bytes) {
  if(bytes == 1){
    byte message = this->a * 1 + this->b * 2 + this->c * 4 + this->d * 8 + this->e * 16 + this->f * 32 + this->g * 64 + this->h * 128;
    Wire.write(message);
  }
}

void DataHandler::request(int device, int bytes = 1){
  if(bytes == 1){
    Wire.requestFrom(device, 1);    // request 1 bytes from slave device #device
    byte received = 0;
    while (Wire.available()) { // only keep last byte
      received = Wire.read(); // receive a byte as character
    }
    this->a = received & 1;
    this->b = received & 2;
    this->c = received & 4;
    this->d = received & 8;
    this->e = received & 16;
    this->f = received & 32;
    this->g = received & 64;
    this->h = received & 128;
  }
  return false;
}

void DataHandler::transmit(){
  
}
