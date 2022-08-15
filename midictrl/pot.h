#ifndef pot_h
#define pot_h
#include "Arduino.h"
#include "iio.h"

class Pot : public Iio {
  public:
    explicit Pot(const int pin, const int samples = 96, const int debounceMs = 32)
      : Iio(IO_TYPE::IO_INPUT)
      , mPin(pin)
      , mSamples(samples)
      , mDebounceMs(debounceMs)
    {
      mValues = new int[mSamples];

      for(int i = 0; i < mSamples; i++){
        mValues[i] = 0;
      }
    }

    void setAnalogMinMax(int aMin, int aMax){
      mAnalogMin = aMin;
      mAnalogMax = aMax;
    }

    void setDigitalMinMax(int aMin, int aMax){
      mDigitalMin = aMin;
      mDigitalMax = aMax;
    }

    virtual void loop() {
      pinMode(mPin, INPUT);

      int latestValue = analogRead(mPin);
      //Serial.println(latestValue);

      mValues[mCurrentIndex] = latestValue;
      mCurrentIndex = (++mCurrentIndex) % mSamples;

      int val = 0;
      for(int i = 0; i < mSamples; i++){
        val += mValues[i];
      }
      val = val / mSamples;

      int valueMapped = map(
        val,
        mAnalogMin, mAnalogMax, 
        mDigitalMin, mDigitalMax
      );

      mCurrentValue = valueMapped;

      if (mCurrentValue != mLastSmoothedFlickerableState) {
        // reset the debouncing timer
        mLastDebounceTime = millis();
        // save the the last flickerable state
        mLastSmoothedFlickerableState = mCurrentValue;
      }

      if (((millis() - mLastDebounceTime) > mDebounceMs) || 
          (abs(mLastSteadyState - mCurrentValue) > 2) 
      ) {

        if(mLastSteadyState != mCurrentValue){
          mChanged = true;
        }
        else{
          mChanged = false;
        }

        mLastSteadyState = mCurrentValue;
      }
  
    }

    virtual bool isChanged(){
      bool change = mChanged;
      mChanged = false;
      return change;
    }

    virtual int getValue(){
      return mLastSteadyState;
    }

    virtual int getPin(){
      return mPin;
    }

  private:
    int mPin = -1;
    int mSamples = 1;
    int mDebounceMs = 10;
    bool mChanged = false;

    int mAnalogMin = 0;
    int mAnalogMax = 8191;

    int mDigitalMin = 0;
    int mDigitalMax = 127;

    int * mValues;
    int mCurrentValue;
    int mCurrentIndex = 0;

    int mLastSmoothedFlickerableState = 0;
    int mLastSteadyState = 0;

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    unsigned long mLastDebounceTime = 0;  // the last time the pot's smoothed value changed
};
#endif