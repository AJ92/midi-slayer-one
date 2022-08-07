#ifndef rot_enc_h
#define rot_enc_h
#include "Arduino.h"
#include "btn.h"
#include "virtualSwitch.h"

class RotEnc;

uint8_t * rotenclist = new uint8_t[sizeof(RotEnc*) * 4];
RotEnc ** ROT_ENC_INTERRUPT_LIST = (RotEnc**)rotenclist;

void readEncoder(int interruptId) {
  RotEnc * rotEnc = ROT_ENC_INTERRUPT_LIST[interruptId];
  Serial.println(interruptId);
  int dtValue = digitalRead(rotEnc->getPinDT());
  if (dtValue == HIGH) {
    Serial.println("Rotated clockwise ⏩");
    rotEnc->getSwitchRight().setPressed(true);
  }
  if (dtValue == LOW) {
    Serial.println("Rotated counterclockwise ⏪");
    rotEnc->getSwitchLeft().setPressed(true);
  }
}

void readEncoder0() {
  readEncoder(0);
}

void readEncoder1() {
  readEncoder(1);
}

void readEncoder1() {
  readEncoder(2);
}

void readEncoder1() {
  readEncoder(3);
}

class RotEnc {
  public:
    /**
     *  @brief Rotary encoder (left/right/buttonpress)
     *  @param pinCLK CLK pin
     *  @param pinDT  DT pin
     *  @param pinSW  SW pin ( button switch ) 
     *  @param debounceMS the button press debounce
     */
    explicit RotEnc(int pinCLK, int pinDT, int pinSW, int interruptId = 0, int debounceMs = 50) 
      : mButton(pinSW, debounceMs)
      , mPinCLK(pinCLK)
      , mPinDT(pinDT)
      , mPinSW(pinSW)
      , mInterruptId(interruptId)
    {
      mSwitchLeft.setValue(1);
      mSwitchRight.setValue(1);

      pinMode(mPinCLK, INPUT);
      pinMode(mPinDT, INPUT);

      ROT_ENC_INTERRUPT_LIST[interruptId] = this;

      switch (interruptId)
      {
      case 0:
        attachInterrupt(digitalPinToInterrupt(mPinCLK), readEncoder0, FALLING);
        break;
      case 1:
        attachInterrupt(digitalPinToInterrupt(mPinCLK), readEncoder1, FALLING);
        break;
      case 2:
        attachInterrupt(digitalPinToInterrupt(mPinCLK), readEncoder2, FALLING);
        break;
      case 3:
        attachInterrupt(digitalPinToInterrupt(mPinCLK), readEncoder3, FALLING);
        break;     
      default:
        break;
      }
      
    }

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


#endif