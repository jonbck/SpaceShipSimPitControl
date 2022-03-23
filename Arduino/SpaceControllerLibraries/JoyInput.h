#include <Arduino.h>
#define JOY_STICK 1
#define JOY_BUTTON 2

class JoyInput {
  
  private:
    int type;
    int pin;
    int low;
    int midlow;
    int midhigh;
    int high;
    int current;
    int sensorvalue;
    bool inputHasChanged;
    
  public:
    JoyInput(int type, int pin, int low = 0, int midlow = 0, int midhigh = 0, int high = 0);
    void getPin();
    void getValue();
    void hasChanged();
    void readValue();
    
};
