#include <Wire.h>
#include "SubController.h"
#include "DataHandler.h"

SubController systemPower(JOY_DATA);
SubController enginePower(JOY_DATA);
SubController weaponsPower(JOY_DATA);
SubController balacePowerDistributionLight(JOY_DATA_LED, 13);
SubController engineBoostLight(JOY_DATA_LED, 11);
SubController silentRunningLight(JOY_DATA_LED, 12);

SubController maxToSystemButton(JOY_MULTIBUTTON, A1, 0, 50);
SubController maxToEnginesButton(JOY_MULTIBUTTON, A2, 0, 50);
SubController maxToWeaponsButton(JOY_MULTIBUTTON, A3, 0, 50);
SubController powerToSystemButton(JOY_MULTIBUTTON, A1, 51, 130);
SubController powerToEnginesButton(JOY_MULTIBUTTON, A2, 51, 130);
SubController powerToWeaponsButton(JOY_MULTIBUTTON, A3, 51, 130);
SubController balacePowerDistributionButton(JOY_MULTIBUTTON, A1, 131, 250);
SubController engineBoostButton(JOY_MULTIBUTTON, A2, 131, 250);
SubController silentRunningButton(JOY_MULTIBUTTON, A3, 131, 250);
SubController bugfix(JOY_MULTIBUTTON, A3, 251, 500);    //Just here for fixing some strange error not recognising the one above (if you figure it out, please tell me)

DataHandler dataHandler(4);

void setup() {
  Wire.begin(SUB_CONTROLLER_POWER_DISTRIBUTION);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);

  //Set pinmode for matrix light rows and columns. (Light utput handled here, not in library)
  for(int i=0; i<11; i++){
    pinMode(i, OUTPUT);
  }
}

void loop() {
  //Read i2c (happens in interrupts)
  //Update accordingly
  
  //Animate special lights
  //Everything off
  for(int col=8; col <11; col++){
    digitalWrite(col, HIGH);
  }
  for(int row=0; row<8; row++){
        digitalWrite(row, LOW);
  }
  
  //System
  digitalWrite(8, LOW);
  for(int row=0; row<systemPower.getValue()+1; row++){
    digitalWrite(row, HIGH);
  }
  delay(1);
  digitalWrite(8, HIGH);
  for(int row=0; row<8; row++){
    digitalWrite(row, LOW);
  }

  //Engine
  digitalWrite(9, LOW);
  for(int row=0; row<enginePower.getValue()+1; row++){
    digitalWrite(row, HIGH);
  }
  delay(1);
  digitalWrite(9, HIGH);
  for(int row=0; row<8; row++){
    digitalWrite(row, LOW);
  }
  
  //Weapons
  digitalWrite(10, LOW);
  for(int row=0; row<weaponsPower.getValue()+1; row++){
    digitalWrite(row, HIGH);
  }
  delay(1);
  digitalWrite(9, HIGH);
  for(int row=0; row<8; row++){
    digitalWrite(row, LOW);
  }

  //Normal lights
  balacePowerDistributionLight.animateLight();
  engineBoostLight.animateLight();
  silentRunningLight.animateLight();

  //Red buttons
  maxToSystemButton.readValue();
  maxToEnginesButton.readValue();
  maxToWeaponsButton.readValue();
  powerToSystemButton.readValue();
  powerToEnginesButton.readValue();
  powerToWeaponsButton.readValue();
  balacePowerDistributionButton.readValue();
  engineBoostButton.readValue();
  silentRunningButton.readValue();

}

void receiveEvent(int bytesReceived) {
  dataHandler.receiveEvent(bytesReceived);

  balacePowerDistributionLight.setValue(dataHandler.a);
  engineBoostLight.setValue(dataHandler.b);
  silentRunningLight.setValue(dataHandler.c);

  systemPower.setValue(dataHandler.message[1]);
  enginePower.setValue(dataHandler.message[2]);
  weaponsPower.setValue(dataHandler.message[3]);

  dataHandler.clearData();

}

void requestEvent() {
  dataHandler.a = maxToSystemButton.getValue();
  dataHandler.b = maxToEnginesButton.getValue();
  dataHandler.c = maxToWeaponsButton.getValue();

  dataHandler.d = powerToSystemButton.getValue();
  dataHandler.e = powerToEnginesButton.getValue();
  dataHandler.f = powerToWeaponsButton.getValue();

  dataHandler.g = balacePowerDistributionButton.getValue();
  dataHandler.h = engineBoostButton.getValue();
  dataHandler.message[1] = silentRunningButton.getValue();
  
  Serial.print(dataHandler.a);
  Serial.print(dataHandler.b);
  Serial.print(dataHandler.c);
  Serial.print(dataHandler.d);
  Serial.print(dataHandler.e);
  Serial.print(dataHandler.f);
  Serial.print(dataHandler.g);
  Serial.print(dataHandler.h);
  Serial.println(dataHandler.message[1]);

  dataHandler.replyEvent(2);
}
