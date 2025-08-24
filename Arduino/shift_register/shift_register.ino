int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
bool countingup = true;

bool tmp = 0;

byte leds[] = {0,0,0};
// 3 bytes to shift register as below:
// leds[0], 0 - 7  --> fuel gauge from low (red) to high (green)
// leds[1], 0      --> fuel gauge high (green)
// leds[1], 1      --> Dock
// leds[1], 2      --> Shield
// leds[1], 3      --> Fuel Scoop
// leds[1], 4      --> Flight Assist Off
// leds[1], 5      --> Silent Running
// leds[1], 6      --> Cargo Scoop
// leds[1], 7      --> Night Vision
// leds[2], 0      --> Orbit Lines
// leds[2], 1      --> Landing Gear
// leds[2], 2      --> Ship Light
// leds[2], 3      --> Half Lights
// leds[2], 4 - 7  --> N/A

int buttons = 0;

void setup(){
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(6, INPUT_PULLUP);   // Silent running - switch
  pinMode(7, INPUT_PULLUP);   //  Flight Assist Off - Switch
  pinMode(8, INPUT_PULLUP);   // Half Lights
  pinMode(9, INPUT_PULLUP);   // Ship Lights
  pinMode(10, INPUT_PULLUP);  // Landing Gear
  pinMode(11, INPUT_PULLUP);  // Orbit Lines
  pinMode(12, INPUT_PULLUP);  // Night Vision
  pinMode(13, INPUT_PULLUP);  // Cargo Scoop

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
}

void loop(){
  if (tmp == 0){
    tmp = 1;
    //bitSet(leds[0],1);
    leds[2] = 255;
  }else{
    tmp = 0;
   //bitClear(leds[0],1);
   leds[2] = 0;
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, leds[2]);
  shiftOut(dataPin, clockPin, MSBFIRST, leds[1]);
  shiftOut(dataPin, clockPin, MSBFIRST, leds[0]);
  digitalWrite(latchPin, HIGH);

  Serial.print(digitalRead(5));
  Serial.print(digitalRead(6));
  Serial.print(digitalRead(7));
  Serial.print(digitalRead(8));
  Serial.print(digitalRead(9));
  Serial.print(digitalRead(10));
  Serial.print(digitalRead(11));
  Serial.print(digitalRead(12));
  Serial.print(digitalRead(13));
  Serial.println();

  delay(500);

}