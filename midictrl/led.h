#ifndef led_h
#define led_h
#include "Arduino.h"
#include "iio.h"

class Led : public Iio{
  public:
    explicit Led(int pin, int intensity = 0)
      : Iio(IO_TYPE::IO_OUTPUT)
      , mPin(pin)
      , mIntensity(intensity)
    {
      
    }

    virtual void loop() {
      pinMode(mPin, OUTPUT);

      digitalWrite(mPin, mIntensity > 127);

      pinMode(mPin, INPUT_PULLDOWN);
    }

    virtual bool isChanged(){
      return false;
    }

    virtual int getValue(){
      return mIntensity;
    }

    virtual void setValue(int value) {
      mIntensity = value;
    }

    virtual int getPin(){
      return mPin;
    }

  private:
    int mPin = -1;
    int mIntensity = 0;

};
#endif