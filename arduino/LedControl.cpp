#include "LedControl.h"

LedController::LedController(){
  pins[0] = 2;
  pins[1] = 3;
  pins[2] = 4;
  pins[3] = 5;
  pins[4] = 6;
  pins[5] = 7;
  pins[6] = 8;
  pins[7] = 9;
}

void LedController::initilize(){
  for(int i = 0; i < getLedAmount(); i++){
    pinMode(pins[i], OUTPUT);
  }
};

void LedController::setLedOn(int ledNr, boolean onOrOff){
  digitalWrite(pins[ledNr], onOrOff ? HIGH : LOW);
};

int LedController::getLedAmount(){
  return 8;
};

String LedController::getCurState(){
  String out = "";
  for(int i = 0; i < getLedAmount(); i++){
     if(pins[i] <= 7)
       out+= String(bitRead(PIND, pins[i]));
     else{
       out+= String(bitRead(PINB, pins[i]-8));
     }
  }
  return out;
}

void LedController::resetLights(){
  for(int i = 0; i < getLedAmount(); i++){
    setLedOn(i, false);
  }
}


