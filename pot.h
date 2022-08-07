#ifndef pot_h
#define pot_h
#include "Arduino.h"
#include "iio.h"

class Pot : public Iio {
  public:
    explicit Pot(const int pin, const int samples = 4)
      : Iio(IO_TYPE::IO_INPUT)
      , mPin(pin)
      , mSamples(samples)
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
      int latestValue = analogRead(mPin);

      int latestValueMapped = map(
        latestValue, 
        mAnalogMin, mAnalogMax, 
        mDigitalMin, mDigitalMax
      );

      mValues[mCurrentIndex] = latestValueMapped;

      int val = 0;
      for(int i = 0; i < mSamples; i++){
        val += mValues[i];
      }
      val = val / mSamples;

      if(val != mCurrentValue){
        mChanged = true;
        mCurrentValue = val;

        Serial.println("The slider moved");
        Serial.println(mCurrentValue);
      }
      else{
        mChanged = false;
      }

      mCurrentIndex = (++mCurrentIndex) % mSamples;
    }

    virtual bool isChanged(){
      return mChanged;
    }

    virtual int getValue(){
      return mCurrentValue;
    }

    virtual int getPin(){
      return mPin;
    }

  private:
    int mPin = -1;
    int mSamples = 4;
    bool mChanged = false;

    int mAnalogMin = 0;
    int mAnalogMax = 1023;

    int mDigitalMin = 0;
    int mDigitalMax = 127;

    int * mValues;
    int mCurrentValue;
    int mCurrentIndex = 0;
};
#endif