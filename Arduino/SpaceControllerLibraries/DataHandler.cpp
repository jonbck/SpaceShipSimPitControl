#include "DataHandler.h"
#include <Wire.h>

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

//Test

DataHandler::DataHandler(int maxMessageSize = 0){
  this->maxMessageSize = maxMessageSize;
  this->message[1];
  if(maxMessageSize <= 0){
      this->message[0] = 0;
  }else{
    this->message[maxMessageSize];
    for(int i=0; i < maxMessageSize; i++){
      this->message[i] = 0;
    }
  }
}

void DataHandler::receiveEvent(int bytesReceived) {
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
  this->message[0] = received;

  int i = 1;
  while (Wire.available() > 0) {
    this->message[i] = Wire.read();
    i++;
  }
}

void DataHandler::replyEvent(int bytes) {
  if(this->a != 0 || this->b != 0 || this->c != 0 || this->d != 0 || this->e != 0 || this->f != 0 || this->g != 0 || this->h != 0){
  //If any of the bit flags are set, treat the first byte as flags
  this->message[0] = 0;
  this->message[0] =  (this->a * 1) +
              (this->b * 2) +
              (this->c * 4) +
              (this->d * 8) +
              (this->e * 16) +
              (this->f * 32) +
              (this->g * 64) +
              (this->h * 128);
  }
  for(int i=0;i<bytes;i++){
     Wire.write(message[i]);
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
  }else if(bytes > 1){
    Wire.requestFrom(device, bytes);    // request 1 bytes from slave device #device
    int i = 0;
    while (Wire.available()) { // only keep last byte
      byte received = Wire.read(); // receive a byte as character
      this->message[i] = received;
      i++;
    }
    this->a = this->message[0] & 1;
    this->b = this->message[0] & 2;
    this->c = this->message[0] & 4;
    this->d = this->message[0] & 8;
    this->e = this->message[0] & 16;
    this->f = this->message[0] & 32;
    this->g = this->message[0] & 64;
    this->h = this->message[0] & 128;
  }
  return false;
}

void DataHandler::transmit(int device, int messageSize = 0){
  if(this->a != 0 || this->b != 0 || this->c != 0 || this->d != 0 || this->e != 0 || this->f != 0 || this->g != 0 || this->h != 0){
    //If any of the bit flags are set, treat the first byte as flags
    this->message[0] = 0;
    this->message[0] =  (this->a * 1) +
                (this->b * 2) +
                (this->c * 4) +
                (this->d * 8) +
                (this->e * 16) +
                (this->f * 32) +
                (this->g * 64) +
                (this->h * 128);
  }
  Wire.beginTransmission(device); // transmit to device
  for(int i=0; i<messageSize; i++){
    Wire.write(this->message[i]); // sends one byte
    Serial.print("x");
  }
  Serial.println();
  Wire.endTransmission();    // stop transmitting
}

void DataHandler::clearData(){
  for(int i=0; i<this->maxMessageSize; i++){
    this->message[i] = 0;
  }
}
