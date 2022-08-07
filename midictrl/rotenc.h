#ifndef rot_enc_h
#define rot_enc_h
#include "Arduino.h"
#include "btn.h"
#include "virtualSwitch.h"

class RotEnc;

void readEncoder(int interruptId);

void readEncoder0();

void readEncoder1();

void readEncoder2();

void readEncoder3();

class RotEnc {
  public:
    /**
     *  @brief Rotary encoder (left/right/buttonpress)
     *  @param pinCLK CLK pin
     *  @param pinDT  DT pin
     *  @param pinSW  SW pin ( button switch ) 
     *  @param debounceMS the button press debounce
     */
    explicit RotEnc(int pinCLK, int pinDT, int pinSW, int interruptId = 0, int debounceMs = 50);

    Btn& getButton(){
      return mButton;
    }

    VirtualSwitch& getSwitchLeft(){
      return mSwitchLeft;
    }

    VirtualSwitch& getSwitchRight(){
      return mSwitchRight;
    }

    int getPinDT(){
      return mPinDT;
    }

  private:
    int mPinCLK;
    int mPinDT;
    int mPinSW;

    int mInterruptId;

    Btn mButton;
    VirtualSwitch mSwitchLeft;
    VirtualSwitch mSwitchRight;
};

namespace {
  uint8_t * rotenclist = new uint8_t[sizeof(RotEnc*) * 4];
  RotEnc ** ROT_ENC_INTERRUPT_LIST = (RotEnc**)rotenclist;
};

#endif