#include "rotenc.h"

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

void readEncoder2() {
  readEncoder(2);
}

void readEncoder3() {
  readEncoder(3);
}

RotEnc::RotEnc(int pinCLK, int pinDT, int pinSW, int interruptId, int debounceMs) 
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