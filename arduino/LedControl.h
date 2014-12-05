#ifndef LedControl_h
#define LedControl_h

#include "Arduino.h"

class LedController{
    private:
      int pins[8];
    public:
      LedController();
      void initilize();
      void setLedOn(int, boolean);
      void resetLights();
      int getLedAmount();		
      String getCurState();
};

#endif
