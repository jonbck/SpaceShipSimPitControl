#include <Arduino.h>

class GameState{

  private:
    //Setup Game State

//    bool fsdJump‬;
//    bool srvHighBeam;
    
  public:  

    // Game environment variables
    bool docked;
    bool landed;
    bool landingGearDown;
    bool shieldsUp;
    bool superCruice;
    bool flightAssistOff;
    bool hardpointsDeployed;
    bool lightsOn;
    bool cargoScoopDeployed;
    bool silentRunning;
    bool scoopingFuel;
    bool fsdMasslocked;
    bool fsdCharging;
    bool fsdCooldown;
    bool lowFuel;
    bool overHeating;
    bool isInDanger;
    bool beingInterdicted;
    bool inMainShip;
    bool inFighter;
    bool inSrv;
    bool hudInAnalysis = true;
    bool nightVision;
    bool fsdJump;
    bool srvHighBeam;
    byte fireGroup;
    byte pipsSys;
    byte pipsEng;
    byte pipsWep;

    // Game controller variables
    bool controllerArm1 = false;
    bool controllerArm2 = false;

    GameState();
    void GameState::testOutput();
    void hasChanged();
    void interpretSerialData(char* key, char* val);
};
