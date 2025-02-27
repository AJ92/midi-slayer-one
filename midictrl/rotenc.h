#ifndef rot_enc_h
#define rot_enc_h
#include "Arduino.h"
#include "iio.h"
#include "btn.h"
#include "virtualswitch.h"

class RotEnc : public Iio{
  public:
    /**
     *  @brief Rotary encoder (left/right/buttonpress)
     *  @param pinCLK CLK pin
     *  @param pinDT  DT pin
     *  @param pinSW  SW pin ( button switch ) 
     *  @param debounceMS the button press debounce
     */
  RotEnc(int pinCLK, int pinDT, int pinSW, int debounceMs = 20) 
      : Iio(IO_TYPE::IO_COMPOSIT)
      , mButton(pinSW, debounceMs)
      , mPinCLK(pinCLK)
      , mPinDT(pinDT)
      , mPinSW(pinSW)
    {
      mSwitchLeft.setValue(1);
      mSwitchRight.setValue(1);

      pinMode(mPinCLK, INPUT);
      pinMode(mPinDT, INPUT);

      mPreviousCLKState = digitalRead(mPinCLK);
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

    virtual void loop() {
      mCurrentCLKState = digitalRead(mPinCLK);

      if(mCurrentCLKState != mPreviousCLKState){
        if(digitalRead(mPinDT) != mCurrentCLKState){
          mSwitchLeft.setPressed(true);
        }
        else{
          mSwitchRight.setPressed(true);
        }
      }

      mPreviousCLKState = mCurrentCLKState;
    }

    virtual bool isChanged() { return false; }



  private:
    int mPinCLK;
    int mPinDT;
    int mPinSW;

    int mCurrentCLKState = 0;
    int mPreviousCLKState = 0;

    Btn mButton;
    VirtualSwitch mSwitchLeft;
    VirtualSwitch mSwitchRight;
};


#endif