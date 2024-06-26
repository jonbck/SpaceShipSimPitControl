// Reading json from computer. Sending keypresses to computer. Intended for use with Node RED and (ED) game input.
// Reading Analog inputs for joysticks and levers
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// ToDo:  Move logic from sub devices (mainly hardpoint switches) to here
//        Prepare to make logic in one place, to make it easier to switch between games/modes
//--------------------------------------------------------------------

#define JSON_SIZE 1000

#include <Wire.h>
#include <Joystick.h>
#include <Mouse.h>
#include <Keyboard.h>
#include "JoyInput.h"
#include "GameState.h"
#include "DataHandler.h"

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

const int noButtonsOnJoystick = 30;

// Create Joystick object

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, noButtonsOnJoystick + 2, 0,     // Buttons, d-pads
                   true, true, true, true, true, true,   // X, Y, Z, Rx, Ry, Rz
                   true, true, false, false, false);     // Rudder see§ms to be same as one of the rotations

// Create Sticks
JoyInput stickX(JOY_STICK, A0, 0, 490, 530, 1020);  // 0; 512; 1023
JoyInput stickY(JOY_STICK, A1, 1020, 560, 520, 0);  // 0; 539; 1023
JoyInput stickZ(JOY_STICK, A2, 0, 310, 350, 770);  // 0; 329; 770
JoyInput stickRx(JOY_STICK, A3, 0, 500, 535, 1020); // 0; 503-530; 1023
JoyInput stickRy(JOY_STICK, A4, 0, 520, 560, 1020); // 0; 537; 1023
JoyInput stickRz(JOY_STICK, A5, 0, 420, 470, 810); // 0; 445; 812
JoyInput stickThrottle(JOY_STICK, A6, 350, 531, 532, 720); // 333; 531; 532; 729

//Create Buttons
JoyInput buttonF1(JOY_BUTTON, 5);
JoyInput buttonF2(JOY_BUTTON, 6);
//Distance buttons


class VirtuJoyButtons {
  private:
    bool current;
    bool inputHasChanged;

  public:
    VirtuJoyButtons() {
      this->current = false;
      this->inputHasChanged = false;
    }

    getValue() {
      return this->current;
    }

    setValue(bool value) {
      if (value != this->current) {
        this->current = value;
        this->inputHasChanged = true;
      }
    }

    hasChanged() {
      if (this->inputHasChanged) {
        this->inputHasChanged = false;
        return true;
      } else {
        return false;
      }
    }
};

VirtuJoyButtons buttons[noButtonsOnJoystick];

// Setup Node RED connections
//Data gatherer
char data[100];
int dataCurrentPosition = 0;
bool readyToInterpret = false;

DataHandler dataHandler(4);

//Setup Gamestate object
GameState gameState;

void setup() {
  // Safety pin. Connect 13 to make sure no mouse/keyboard messes up when reprogramming.
  pinMode(13, INPUT_PULLUP);
  while (digitalRead(13) == LOW) {  }

  //  Mouse.end();
  //  Keyboard.end();
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);

  // Setup slave controllers

  // Setup local joysticks and buttons
  Joystick.setXAxisRange(0, 255);
  Joystick.setYAxisRange(0, 255);
  Joystick.setZAxisRange(0, 255);
  Joystick.setRxAxisRange(0, 255);
  Joystick.setRyAxisRange(0, 255);
  Joystick.setRzAxisRange(0, 255);
  Joystick.setThrottleRange(0, 255);
  //  Joystick.setRudderRange(0, 255);      // This probably would clash with one of the above
  //  Joystick.setAcceleratorRange(0, 255);
  //  Joystick.setBrakeRange(0, 255);
  //  Joystick.setSteeringRange(0, 255);

  // Setup gamepad(s)
  Joystick.begin();

}

void loop() {
  // Read input changes from devices
  dataHandler.request(SUB_CONTROLLER_FSD, 1);
  buttons[0].setValue(dataHandler.a);
  buttons[1].setValue(dataHandler.b);
  buttons[2].setValue(dataHandler.c);
  buttons[3].setValue(dataHandler.d);

  dataHandler.request(SUB_CONTROLLER_HARDPOINTS, 1);
  buttons[4].setValue(dataHandler.a);
  buttons[5].setValue(dataHandler.b);
  gameState.controllerArm1 = dataHandler.c;
  gameState.controllerArm2 = dataHandler.d;
  buttons[6].setValue(dataHandler.e);
  if(gameState.hudInAnalysis == dataHandler.f){
    buttons[7].setValue(!dataHandler.f);
  }
  buttons[8].setValue(dataHandler.g);
  buttons[9].setValue(dataHandler.h);
  
  dataHandler.request(SUB_CONTROLLER_POWER_DISTRIBUTION, 2);
  buttons[10].setValue(dataHandler.a);
  buttons[11].setValue(dataHandler.b);
  buttons[12].setValue(dataHandler.c);
  buttons[13].setValue(dataHandler.d);
  buttons[14].setValue(dataHandler.e);
  buttons[15].setValue(dataHandler.f);
  buttons[16].setValue(dataHandler.g);
  buttons[17].setValue(dataHandler.h);
  buttons[18].setValue(dataHandler.message[1]);

dataHandler.request(SUB_CONTROLLER_FUEL, 1);
  buttons[19].setValue(dataHandler.a);
  buttons[20].setValue(dataHandler.b);
  buttons[21].setValue(dataHandler.c);
  buttons[22].setValue(dataHandler.d);
  buttons[23].setValue(dataHandler.e);
  buttons[24].setValue(dataHandler.f);
  buttons[25].setValue(dataHandler.g);
  buttons[26].setValue(dataHandler.h);

  //Do this for every device
  //Either just add it to the joystick, or add a function to detect changes and then add it to the joystick.

  // Read input from directly connected buttons and joysticks
  readJoyStickPinValues();

  // Act on input changes from specialist devices

  // Update gamepad output
  setJoyStickValues();

  // add changes from slave devices,

  // Read game state from NodeRED
  gameStateFromNodeRED();

  //Update state object

  // Output data to secondary devices
  // FSD
  byte sendByte = 0;
  sendByte =  (gameState.fsdCooldown * 1) +
              (gameState.fsdMasslocked * 2) +
              (gameState.fsdCharging * 4) +
              (gameState.superCruice * 8) +
              (gameState.fsdJump * 16);

  Wire.beginTransmission(SUB_CONTROLLER_FSD); // transmit to device #8
  Wire.write(sendByte);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  //HARDPOINTS
  sendByte = 0;
  sendByte =  (gameState.hudInAnalysis * 1) +
              (gameState.hardpointsDeployed * 2) +
              (gameState.controllerArm1 * 4) +
              (gameState.controllerArm2 * 8);

  Wire.beginTransmission(SUB_CONTROLLER_HARDPOINTS); // transmit to device #8
  Wire.write(sendByte);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  //POWER DISTRIBUTION
  sendByte = 0;
  sendByte =  (0 * 1) +                               // Power distribution Light
              (0 * 2) +                               // Engine Boost Light
              (gameState.silentRunning * 4);

  Wire.beginTransmission(SUB_CONTROLLER_POWER_DISTRIBUTION); // transmit to device
  Wire.write(sendByte);              // sends one byte
  Wire.write(gameState.pipsSys);              // sends one byte
  Wire.write(gameState.pipsEng);              // sends one byte
  Wire.write(gameState.pipsWep);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  
  //EXTERNAL
  sendByte = 0;
  sendByte =  (gameState.isInDanger * 1) +
              (gameState.overHeating * 2);

  Wire.beginTransmission(SUB_CONTROLLER_BOARD_EXTERNAL); // transmit to device #8
  Wire.write(sendByte);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  // FUEL
  byte sendByte = 0;
  sendByte =  (gameState.docked * 1) +
              (gameState.shieldsUp * 2) +
              (gameState.scoopingFuel * 4) +
              (gameState.flightAssistOff * 8) +
              (gameState.silentRunning * 16) + 
              (gameState.cargoScoopDeployed * 32) + 
              (gameState.nightVision * 64) + 
              (0 * 128); // Orbit lines not implemented in data gathering 

  byte sendByte2 = 0;
  sendByte2 = (gameState.landingGearDown * 1) + 
              (gameState.lightsOn * 2);
  if(gameState.inSrv && !gameState.srvHighBeam){
    sendByte2 = sendByte2 + 4;
  }
  byte sendByte3 = 0;
  // ADD CODE FOR SENDING AMOUNT OF FUEL

  Wire.beginTransmission(SUB_CONTROLLER_FSD); // transmit to device #8
  Wire.write(sendByte);              // sends one byte
  Wire.write(sendByte2);              // sends one byte
  Wire.write(sendByte3);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  delay(50);
}

void readJoyStickPinValues() {
  stickX.readValue();
  stickY.readValue();
  stickZ.readValue();
  stickRx.readValue();
  stickRy.readValue();
  stickRz.readValue();
  stickThrottle.readValue();

  buttonF1.readValue();
  buttonF2.readValue();
}

void setJoyStickValues() {
  Joystick.setXAxis(stickX.getValue());
  Joystick.setYAxis(stickY.getValue());
  Joystick.setZAxis(stickZ.getValue());
  Joystick.setRxAxis(stickRx.getValue());
  Joystick.setRyAxis(stickRy.getValue());
  Joystick.setRzAxis(stickRz.getValue());
  Joystick.setThrottle(stickThrottle.getValue());
  
  if (buttonF1.hasChanged()) {
    Joystick.setButton(0, buttonF1.getValue());
    Serial.print("QWERTY");
  }
  if (buttonF2.hasChanged()) {
    Joystick.setButton(1, buttonF2.getValue());
    Serial.print("QWERTY");
  }

  for (int i = 0; i < arr_len(buttons); i++) {
    //Serial.print(buttons[i].getValue());
    if (buttons[i].hasChanged()) {
      Joystick.setButton(i + 2, buttons[i].getValue());
      //Serial.print(i);
      //Serial.print(": ");
      //Serial.println(buttons[i].getValue());
    }
  }
}

void gameStateFromNodeRED() {
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      data[dataCurrentPosition] = Serial.read();
      //If a new one starts. Start over.
      if (data[dataCurrentPosition] == '{') {
        for(int i = 0; i < arr_len(data); i++){
          data[i] = ' ';
        }
        dataCurrentPosition = 0;
      }

      //If end reached, prepare for interpretation
      if (data[dataCurrentPosition] == '}' || data[dataCurrentPosition] == ',') {
        readyToInterpret = true;
        break;
      }
      dataCurrentPosition++;
      //Instead of overflow, start over
      if (dataCurrentPosition > arr_len(data)) {
        dataCurrentPosition = 0;
      }
    }

    if (readyToInterpret) {
      readyToInterpret = false;
      char key[arr_len(data)] = "";
      char val[arr_len(data)] = "";
      int keyCurrentPosition = 0;
      int valCurrentPosition = 0;
      int breakPlacement = 0;

      //Find the break point
      for (int i = 0; i < dataCurrentPosition; i++) {
        if (data[i] == ':') {
          breakPlacement = i;
          break;
        }
      }
      if (breakPlacement != 0) {
        for (int i = 0; i < breakPlacement; i++) {
          if(data[i] != '{' && data[i] != ' ' && data[i] != ',' && data[i] != '\"' && data[i] != '[' && data[i] != ']'){
            key[keyCurrentPosition] = data[i];
            keyCurrentPosition++;
          }
        }
        for (int i = breakPlacement+1; i < dataCurrentPosition; i++) {
          if(data[i] == '}' || data[i] == ','){ 
            break;
          }
          if(data[i] != '{' && data[i] != ' ' && data[i] != ',' && data[i] != '\"' && data[i] != '[' && data[i] != ']'){
            val[valCurrentPosition] = data[i];
            valCurrentPosition++;
          }
        }
        Serial.print("[ ");
        Serial.print(key);
        Serial.print(" | ");
        Serial.print(val);
        Serial.println(" ]");
        
        gameState.interpretSerialData(key, val);      // Function in Game State
        Serial.print(gameState.pipsSys);
        Serial.print(gameState.pipsEng);
        Serial.println(gameState.pipsWep);

      }
      for(int i = 0; i < arr_len(data); i++){
        data[i] = ' ';
      }
      dataCurrentPosition = 0;
      breakPlacement = 0;
      key[0]='\0';
      val[0]='\0';
    }
  }  
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
