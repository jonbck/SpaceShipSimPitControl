#include <Arduino.h>
#define SUB_CONTROLLER 8
#define SUB_CONTROLLER_FSD 8
#define SUB_CONTROLLER_HARDPOINTS 9
#define SUB_CONTROLLER_POWER_DISTRIBUTION 10
#define SUB_CONTROLLER_BOARD_EXTERNAL 11

class DataHandler {
  
  private:
    int maxMessageSize;
    
  public:
    bool a = false;
    bool b = false;
    bool c = false;
    bool d = false;
    bool e = false;
    bool f = false;
    bool g = false;
    bool h = false;
    byte message[];
    
    DataHandler(int maxMessageSize);
    void DataHandler::receiveEvent(int bytesReceived);
    void DataHandler::replyEvent(int bytes);
    void DataHandler::request(int device, int bytes);
    void DataHandler::transmit(int device, int messageSize = 0);
    void DataHandler::clearData();
};
