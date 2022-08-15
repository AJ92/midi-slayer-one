#ifndef multiplexer_h
#define multiplexer_h
#include "Arduino.h"

class Multiplexer {
  public:
    Multiplexer(
      const int pinSignal, 
      const int pinEnable,
      const int pinS0,
      const int pinS1,
      const int pinS2,
      const int pinS3
    )
      : mPin(pinSignal)
      , mPinEnable(pinEnable)
      , mPinS0(pinS0)
      , mPinS1(pinS1)
      , mPinS2(pinS2)
      , mPinS3(pinS3)
    {
      mChannelSelectPins = new int[4];
      mChannelSelectPins[0] = mPinS0;
      mChannelSelectPins[1] = mPinS1;
      mChannelSelectPins[2] = mPinS2;
      mChannelSelectPins[3] = mPinS3;

      pinMode(mPinS0, OUTPUT); 
      pinMode(mPinS1, OUTPUT); 
      pinMode(mPinS2, OUTPUT); 
      pinMode(mPinS3, OUTPUT); 

      // 16 possible inputs/outputs addressed via 4 channel selection bits (pins)
      mChannelValues = new int[16 * 4];

      for(int channel = 0; channel < 16; channel++){
        initChannelValue(channel);
      }
    }

    void setChannel(int channel) {
      for(int i = 0; i < 4; i ++){
        digitalWrite(mChannelSelectPins[i], mChannelValues[channel * 4 + i]);
      }
    }

  private:

    void initChannelValue(int channel) {
      int channelBase = 4 * channel;
      mChannelValues[channelBase + 0] = ((channel & 0b0001) == 0b0001);
      mChannelValues[channelBase + 1] = ((channel & 0b0010) == 0b0010);
      mChannelValues[channelBase + 2] = ((channel & 0b0100) == 0b0100);
      mChannelValues[channelBase + 3] = ((channel & 0b1000) == 0b1000);
    }

    const int mPin;
    const int mPinEnable;
    const int mPinS0;
    const int mPinS1;
    const int mPinS2;
    const int mPinS3;

    int * mChannelSelectPins;
    int * mChannelValues;

};
#endif