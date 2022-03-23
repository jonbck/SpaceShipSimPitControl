#include "GameState.h";

GameState::GameState(){
}

void GameState::hasChanged(){
}

void GameState::testOutput(){
/*
  Serial.print(this->landed);
  Serial.print(this->landingGearDown);
  Serial.print(this->shieldsUp);
  Serial.print(this->superCruice);
  Serial.print(this->flightAssistOff);
  Serial.print(this->hardpointsDeployed);
  Serial.print(this->lightsOn);
  Serial.print(this->cargoScoopDeployed);
  Serial.print(this->silentRunning);
  Serial.print(this->scoopingFuel);
  Serial.print(this->fsdMasslocked);
  Serial.print(this->fsdCharging);
  Serial.print(this->fsdCooldown);
  Serial.print(this->lowFuel);
  Serial.print(this->overHeating);
  Serial.print(this->isInDanger);
  Serial.print(this->beingInterdicted);
  Serial.print(this->inMainShip);
  Serial.print(this->inFighter);
  Serial.print(this->inSrv);
  Serial.print(this->hudInAnalysis);
  Serial.print(this->nightVision);
  Serial.print(this->fsdJump);
  Serial.println(this->srvHighBeam);
  Serial.print("Firegroup: ");
  Serial.println(this->fireGroup);
*/
}

void GameState::interpretSerialData(char* key, char* val){
  if(String(key).equalsIgnoreCase("flags")){
    long output = String(val).toInt();
    
    this->docked = output & 1;
    this->landed = output & 2;
    this->landingGearDown = output & 4;
    this->shieldsUp = output & 8;
    this->superCruice = output & 16;
    this->flightAssistOff = output & 32;
    this->hardpointsDeployed = output & 64;
    this->lightsOn = output & 256;
    this->cargoScoopDeployed = output & 512;
    this->silentRunning = output & 1024;
    this->scoopingFuel = output & 2048;
    this->fsdMasslocked = output & 65536;
    this->fsdCharging = output & 131072;
    this->fsdCooldown = output & 262144;
    this->lowFuel = output & 524288;
    this->overHeating = output & 1048576;
    this->isInDanger = output & 4194304;
    this->beingInterdicted = output & 8388608;
    this->inMainShip = output & 16777216;
    this->inFighter = output & 33554432;
    this->inSrv = output & 33554432;
    this->hudInAnalysis = output & 134217728;
    this->nightVision = output & 268435456;
    this->fsdJump = output & 1073741824;
    this->srvHighBeam = output & 2147483648;
  }
  if(String(key).equalsIgnoreCase("flags2")){
  }
  if(String(key).equalsIgnoreCase("firegroup")){
    this->fireGroup = String(val).toInt();
  }
}
