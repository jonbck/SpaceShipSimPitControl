#include <Arduino.h>
#define JOY_STICK 1
#define JOY_BUTTON 2
#define JOY_LED 3
#define JOY_DATA_LED 4

#define JOY_LED_ON 1
#define JOY_LED_OFF 0
#define JOY_LED_BLINK 2
#define JOY_LED_BLINK_FAST 3
#define JOY_LED_PULSATE 4

class SubController {
  
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
    int ledMode;
    long timestamp;
    int animationTime;
    int animationFrame;
    
  public:
    SubController(int type, int pin, int low = 0, int midlow = 0, int midhigh = 0, int high = 0);
    void getPin();
    int getValue();
    bool hasChanged();
    void readValue();
    void setValue(int newValue);
    void setLight();
    void animateLight();
    
};
