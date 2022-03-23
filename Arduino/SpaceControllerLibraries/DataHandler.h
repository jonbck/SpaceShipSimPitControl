#include <Arduino.h>
#define SUB_CONTROLLER 8
#define SUB_CONTROLLER_FSD 8

class DataHandler {
  
  private:
    int example;
    
  public:
    bool a = false;
    bool b = false;
    bool c = false;
    bool d = false;
    bool e = false;
    bool f = false;
    bool g = false;
    bool h = false;
    
    DataHandler(int example);
    int setExample(int example);
    void DataHandler::receiveEvent(int bytesReceived);
    void DataHandler::replyEvent(int bytes);
    void DataHandler::request(int device, int bytes);
    void DataHandler::transmit();
};
