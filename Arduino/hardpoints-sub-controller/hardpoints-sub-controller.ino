/*
 * ToDo:  HUD registers "backwards" (it's pressed until switched on). Turn it around
 *        Move logic to the databroker. Just handle buttonpresses and lights here.
 *          Mainly, when a switch is switched. Change it's value instead of sending a keypress depending on relation to light. Move light stuff to databroker
 */

#include <Wire.h>
#include "SubController.h"
#include "DataHandler.h"

SubController hudLight(JOY_DATA_LED, 4);
SubController hardpointsLight(JOY_DATA_LED, 5);
SubController arm1Light(JOY_DATA_LED, 6);
SubController arm2Light(JOY_DATA_LED, 7);
SubController heatSinkLight(JOY_DATA_LED, 1);
SubController shieldCellLight(JOY_DATA_LED, 13);
SubController chaffLauncherLight(JOY_DATA_LED, A0);
SubController chargeEcmLight(JOY_DATA_LED, A1);

SubController hudSwitch(JOY_BUTTON, 8);
SubController hardpointsSwitch(JOY_BUTTON, 9);
SubController arm1Switch(JOY_BUTTON, 11);
SubController arm2Switch(JOY_BUTTON, 12);
SubController heatSinkButton(JOY_BUTTON, A2);
SubController shieldCellButton(JOY_BUTTON, A3);
SubController chaffLauncherButton(JOY_BUTTON, A4);
SubController chargeEcmButton(JOY_BUTTON, A5);
DataHandler dataHandler(12);

void setup() {
  Wire.begin(SUB_CONTROLLER_HARDPOINTS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
}

void loop() {
  //Read i2c (happens in interrupts)
  //Update accordingly
  hudLight.animateLight();
  hardpointsLight.animateLight();
  arm1Light.animateLight();
  arm2Light.animateLight();
  heatSinkLight.animateLight();
  shieldCellLight.animateLight();
  chaffLauncherLight.animateLight();
  chargeEcmLight.animateLight();

  //Read buttons
  hudSwitch.readValue();
  hardpointsSwitch.readValue();
  arm1Switch.readValue();
  arm2Switch.readValue();
  heatSinkButton.readValue();
  shieldCellButton.readValue();
  chaffLauncherButton.readValue();
  chargeEcmButton.readValue();

  
  delay(10);
}

void receiveEvent(int bytesReceived) {
  dataHandler.receiveEvent(bytesReceived);

  hudLight.setValue(dataHandler.a);
  hardpointsLight.setValue(dataHandler.b);
  arm1Light.setValue(dataHandler.c);
  arm2Light.setValue(dataHandler.d);
  heatSinkLight.setValue(dataHandler.e);
  shieldCellLight.setValue(dataHandler.f);
  chaffLauncherLight.setValue(dataHandler.g);
  chargeEcmLight.setValue(dataHandler.h);
}

void requestEvent() {
  if(hudLight.getValue() == hudSwitch.getValue()){
      dataHandler.a = true;
  }else{
      dataHandler.a = false;    
  }
  if(hardpointsLight.getValue() != hardpointsSwitch.getValue()){
      dataHandler.b = true;
  }else{
      dataHandler.b = false;    
  }
  dataHandler.c = arm1Switch.getValue();
  dataHandler.d = arm2Switch.getValue();
  dataHandler.e = heatSinkButton.getValue();
  dataHandler.f = shieldCellButton.getValue();
  dataHandler.g = chaffLauncherButton.getValue();
  dataHandler.h = chargeEcmButton.getValue();

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
