#ifndef iperipheral_h
#define iperipheral_h
#include "Arduino.h"

enum class IO_TYPE : uint8_t
{
  IO_INPUT = 0,
  IO_OUTPUT = 1,
  IO_COMPOSIT = 2,
  IO_COUNT = 3
};

class Iio {
  public:
    Iio(const IO_TYPE t) : mType(t) {}

    virtual void loop() = 0;
    virtual bool isChanged() = 0;

    //for intput
    virtual int getValue() { return 0; }

    // for output
    virtual int getPin() { return -1; }
    virtual void setValue(int value) {}

    virtual int readCalibration(int address) { return 0; }
    virtual int writeCalibration(int address) { return 0; }
    virtual void calibrate(bool enabled) {};

    const IO_TYPE getType() { return mType; }

  private:
    const IO_TYPE mType;
};
#endif