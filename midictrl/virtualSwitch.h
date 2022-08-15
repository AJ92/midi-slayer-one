#ifndef virtual_switch_h
#define virtual_switch_h
#include "Arduino.h"
#include "iio.h"

class VirtualSwitch : public Iio{
  public:
    explicit VirtualSwitch()
      : Iio(IO_TYPE::IO_INPUT)
    {
    }

    virtual void loop() {
      if(mMarkForDePress && mPressed){
        setPressed(false);
      }

      if(mPressed){
        mMarkForDePress = true;
      }
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
      return mValue;
    }

    virtual int getPin(){
      return -1;
    }

    void setValue(int value){
      mValue = value;
    }

    void setPressed(bool pressed){
      if(pressed){
        mMarkForDePress = false;
      }

      if(mPressed != pressed){
        mChanged = true;
      }
      else{
        mChanged = false;
      }

      if(mPressed && (!pressed)){
        mReleased = true;
        mMarkForDePress = false;
      }
      mPressed = pressed;
    }

  private:
    bool mPressed = false;
    bool mReleased = false;
    bool mChanged = false;
    int mValue = 0;

    bool mMarkForDePress = false;
};
#endif