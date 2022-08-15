#ifndef btn_h
#define btn_h
#include "Arduino.h"
#include "iio.h"

class Btn : public Iio{
  public:
    explicit Btn(const int pin, const int debounceMs = 32)
      : Iio(IO_TYPE::IO_INPUT)
      , mPin(pin)
      , mDebounceMs(debounceMs)
    {
      
    }

    virtual void loop() {
      pinMode(mPin, INPUT_PULLUP);

      //Serial.print("btn loop ");

      mCurrentState = digitalRead(mPin);
      //Serial.println(mCurrentState);

      // check to see if you just pressed the button
      // (i.e. the input went from LOW to HIGH), and you've waited long enough
      // since the last press to ignore any noise:

      // If the switch/button changed, due to noise or pressing:
      if (mCurrentState != mLastFlickerableState) {
        // reset the debouncing timer
        mLastDebounceTime = millis();
        // save the the last flickerable state
        mLastFlickerableState = mCurrentState;
      }

      if ((millis() - mLastDebounceTime) > mDebounceMs) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (mLastSteadyState == HIGH && mCurrentState == LOW){
          //Serial.println("The button is pressed");
          mChanged = true;
        }
        else if (mLastSteadyState == LOW && mCurrentState == HIGH){
          //Serial.println("The button is released");
          mChanged = true;
        }
        else{
          mChanged = false;
        }

        // save the the last steady state
        mLastSteadyState = mCurrentState;
      }

      if(mLastSteadyState == HIGH){
        if(mPressed){
          mReleased = true;
        }
        mPressed = false;
      }
      else{
        mPressed = true;
      }

      //pinMode(mPin, INPUT_PULLDOWN);
    }

    bool isPressed(){
      return mPressed;
    }

    bool isReleased(){
      bool released = mReleased;
      mReleased = false;
      return released;
    }

    virtual bool isChanged(){
      bool change = mChanged;
      mChanged = false;
      return change;
    }

    virtual int getValue(){
      if(mLastSteadyState == HIGH){
        return 0; // release
      }
      return 1; // press
    }

    virtual int getPin(){
      return mPin;
    }

  private:
    int mPin = -1;
    int mDebounceMs = 10;
    bool mPressed = false;
    bool mReleased = false;
    bool mChanged = false;

    // Variables will change:
    int mLastSteadyState = LOW;       // the previous steady state from the input pin
    int mLastFlickerableState = LOW;  // the previous flickerable state from the input pin
    int mCurrentState = LOW;                // the current reading from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    unsigned long mLastDebounceTime = 0;  // the last time the output pin was toggled
};
#endif