int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
bool countingup = true;

byte leds[] = {0,0,0};
int buttons = 0;

void setup(){
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0);
  digitalWrite(latchPin, HIGH);
}

void loop(){
  for (int x = 0; x < 24; x++){
    for(int y = 5; y < 14; y++){
        bitWrite(buttons, y-5, digitalRead(y));
    }
    if(countingup){
      if(x<8){
        bitSet(leds[0],x);
      }else if(x<16){
        bitSet(leds[1],x-8);
      }else{
        bitSet(leds[2],x-16);
        if(x==23){
          countingup = false;
        }
      }
    }else{
            if(x<8){
        bitClear(leds[0],x);
      }else if(x<16){
        bitClear(leds[1],x-8);
      }else{
        bitClear(leds[2],x-16);
        if(x==23){
          countingup = true;
        }
      }
    }

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, leds[2]);
    shiftOut(dataPin, clockPin, MSBFIRST, leds[1]);
    shiftOut(dataPin, clockPin, MSBFIRST, leds[0]);
    digitalWrite(latchPin, HIGH);
    for(int z = 0; z<8; z++){
      Serial.print(bitRead(leds[2], z));
    }
    Serial.println();
    delay(1000);
  }
}