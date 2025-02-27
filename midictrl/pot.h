#ifndef pot_h
#define pot_h
#include "Arduino.h"
#include "iio.h"

class Pot : public Iio {
  public:
    explicit Pot(const int pin, const int samples = 96, const int debounceMs = 32)
        : Iio(IO_TYPE::IO_INPUT), mPin(pin), mSamples(samples), mDebounceMs(debounceMs), mFirstRead(true)
    {
      mValues = new int[mSamples];
    }

    virtual void loop()
    {
      pinMode(mPin, INPUT);

      int analogValue = analogRead(mPin);

      if (mFirstRead)
      {
        // init with value
        int valueMapped = mapPotentiometer(analogValue,
                                           mCalibration.min, mCalibration.center, mCalibration.max,
                                           mDigitalMin, mDigitalCenter, mDigitalMax);

        for (int i = 0; i < mSamples; i++)
        {
          mValues[i] = valueMapped;
        }

        mFirstRead = false;
      }

      mValues[mCurrentIndex] = analogValue;
      mCurrentIndex = (++mCurrentIndex) % mSamples;

      int smoothedAnalogValue = 0;
      for (int i = 0; i < mSamples; i++)
      {
        smoothedAnalogValue += mValues[i];
      }
      mCurrentAnalogValue = smoothedAnalogValue / mSamples;

      int valueMapped = mapPotentiometer(mCurrentAnalogValue,
                                         mCalibration.min, mCalibration.center, mCalibration.max,
                                         mDigitalMin, mDigitalCenter, mDigitalMax);

      mCurrentValue = valueMapped;

      if (mCurrentValue != mLastSmoothedFlickerableState)
      {
        // reset the debouncing timer
        mLastDebounceTime = millis();
        // save the the last flickerable state
        mLastSmoothedFlickerableState = mCurrentValue;
      }

      if (((millis() - mLastDebounceTime) > mDebounceMs) ||
          (abs(mLastSteadyState - mCurrentValue) > 2))
      {

        if (mLastSteadyState != mCurrentValue)
        {
          mChanged = true;
        }
        else
        {
          mChanged = false;
        }

        mLastSteadyState = mCurrentValue;
      }

      if (mCalibrationEnabled)
      {
        if (mCurrentAnalogValue < mCalibration.min)
        {
          mCalibration.min = mCurrentAnalogValue;
        }
        if (mCurrentAnalogValue > mCalibration.max)
        {
          mCalibration.max = mCurrentAnalogValue;
        }
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

    virtual int readCalibration(int address)
    {
      int offset = 0;
      EEPROM.get(address + offset, mCalibration.min);
      offset += sizeof(int);
      EEPROM.get(address + offset, mCalibration.max);
      offset += sizeof(int);
      EEPROM.get(address + offset, mCalibration.center);
      offset += sizeof(int);
      return offset;
    }

    virtual int writeCalibration(int address)
    {
      mCalibration.center = mCurrentAnalogValue;

      int offset = 0;
      EEPROM.put(address + offset, mCalibration.min);
      offset += sizeof(int);
      EEPROM.put(address + offset, mCalibration.max);
      offset += sizeof(int);
      EEPROM.put(address + offset, mCalibration.center);
      offset += sizeof(int);
      return offset;
    }

    // starts, stops calibration
    // running calibration just adjusts the potentiometer's ends.
    // center is set if calibration is written to eeprom...
    virtual void calibrate(bool enabled)
    {
      mCalibrationEnabled = enabled;

      if (enabled)
      {
        // reset ends.
        mCalibration.min = 8192;
        mCalibration.max = 0;
      }
    }

  private:
    int mapPotentiometer(int value, int minVal, int centerVal, int maxVal, int toMin, int toCenter, int toMax)
    {
      // If value is less than or equal to the center, map it from [minVal, centerVal] to [toMin, toCenter]
      if (value <= mCalibration.center)
      {
        return map(value, minVal, centerVal, toMin, toCenter);
      }
      // If value is greater than the center, map it from [centerVal, maxVal] to [toCenter, toMax]
      else
      {
        return map(value, centerVal, maxVal, toCenter, toMax);
      }
    }

    // analog values
    struct PotCalibration
    {
      int min = 20;
      int max = 8172;
      int center = 4096;
    } mCalibration;

    bool mCalibrationEnabled = false;

    int mPin = -1;
    int mSamples = 1;
    int mDebounceMs = 10;
    bool mFirstRead = true;

    bool mChanged = false;

    int mDigitalMin = 0;
    int mDigitalMax = 127;
    int mDigitalCenter = 63;

    // analog values
    int * mValues;
    // current analog, filtered, debounced value
    int mCurrentAnalogValue = 0;
    // current digital, filtered, debounced value
    int mCurrentValue = 0;
    int mCurrentIndex = 0;

    int mLastSmoothedFlickerableState = 0;
    int mLastSteadyState = 0;

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    unsigned long mLastDebounceTime = 0;  // the last time the pot's smoothed value changed
};
#endif