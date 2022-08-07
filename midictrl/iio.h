#ifndef iperipheral_h
#define iperipheral_h
#include "Arduino.h"

enum class IO_TYPE: uint8_t {
  IO_INPUT = 0,
  IO_OUTPUT = 1,
  IO_COUNT = 2
};

class Iio {
  public:

    Iio(const IO_TYPE t) : mType(t) { }

    virtual void loop() = 0;
    virtual bool isChanged() = 0;

    //for intput
    virtual int getValue() = 0;

    // for output
    virtual int getPin() = 0;
    virtual void setValue(int value) {}

    const IO_TYPE getType() { return mType; }

  private:
    const IO_TYPE mType;

};
#endif