#ifndef controller_h
#define controller_h
#include "Arduino.h"
#include "multiplexer.h"
#include "iio.h"
#include "midiusb.h"
#include <EEPROM.h>

#include "driver/adc.h"

class Controller{
  public:
    /**
     * \brief Initializes the midi controller with
     * \param[in] maxIos maximum count of IOs that can be added.addIo
     */
    Controller(const int maxIos)
    {
      uint8_t * ios = new uint8_t[sizeof(IOStruct*) * mMaxIos];
      mIoList = (IOStruct**)ios;

      for(int i = 0; i < mMaxIos; i++){
        IOStruct * ios = new IOStruct();
        mIoList[i] = ios;
      }

      mMidi.manufacturer("vtx-slayer");
      mMidi.product("midi-slayer-one");

      //midi.serial("0001");  // serial number SN
      //midi.revision(1); // product revison
      //midi.deviceID(uint16_t VID, uint16_t PID);

      // 5 is default but does not work... 1 does!
      mMidi.setBaseEP(0x01);
      mMidi.begin();
    }

    /**
     * \brief Adds a new io device.
     */
    void addIo(Iio * io, Multiplexer * multiplexer = nullptr, int multiplexerChannel = -1){
      IOStruct * ios = new IOStruct();
      ios->io = io;
      ios->multiplexer = multiplexer;
      ios->multiplexerChannel = multiplexerChannel;

      mIoList[mIoCount] = ios;

      mIoCount++;
    }

    /**
     * \brief Call after all IOs have been added.
     */
    void initialize()
    {
      readEeprom();
    }

    void calibrate(bool enabled)
    {
      // start/stop calibration for all IOs
      for (int i = 0; i < mIoCount; i++)
      {
        IOStruct *ioStruct = mIoList[i];
        Iio *io = ioStruct->io;
        io->calibrate(enabled);
      }

      if (!enabled)
      {
        writeEeprom();
      }
    }

    void loop(){
      //get the values as fast as possible
      inputLoop();
      midiLoop();
    };

    void inputLoop(){
      //max analog rot pot value 8191.0
      //max digital output value 127.0

      for(int i = 0; i < mIoCount; i++){
        IOStruct * ioStruct = mIoList[i];
        Iio * io = ioStruct->io;
        // if a multiplexer is assigned, set it up first
        Multiplexer * multiplexer = ioStruct->multiplexer;

        if(multiplexer != nullptr){
          multiplexer->setChannel(ioStruct->multiplexerChannel);
        }

        io->loop();
        if(io->getType() == IO_TYPE::IO_INPUT){
          if(io->isChanged()){
            uint8_t controller = 1 + static_cast<uint8_t>(i);
            uint8_t value = static_cast<uint8_t>(io->getValue());

            mMidiController[mCurrentMessage] = controller;
            mMidiValues[mCurrentMessage] = value;

            mCurrentMessage++;
            if(mCurrentMessage >= mMaxMessageBuffer){
              mCurrentMessage = 0;
            }

            if(mCurrentMessage == mWrittenMessage){
              Serial.println("buffer overflow");
            }
          }
        }
        else{
          // OUTPUT current not implemented...
        }
      }
    }

    void midiLoop(){
      while(mWrittenMessage != mCurrentMessage){
        sendValue( mMidiValues[mWrittenMessage], mMidiController[mWrittenMessage], mChannel);
        mWrittenMessage++;
        if(mWrittenMessage >= mMaxMessageBuffer){
          mWrittenMessage = 0;
        }
      }
    }

  private:
    void sendValue(uint8_t value, uint8_t controller, uint8_t channel)
    {
      uint8_t buf[] = {0xb0, controller, value};
      tud_midi_stream_write(channel, buf, 3);
    }

    // iterate over IOs to read their calibration
    void readEeprom()
    {
      if (!EEPROM.begin(1024))
      {
        Serial.println("Failed to initialise EEPROM");
        return;
      }

      int address = 0;
      for (int i = 0; i < mIoCount; i++)
      {
        IOStruct *ioStruct = mIoList[i];
        Iio *io = ioStruct->io;
        address += io->readCalibration(address);
      }
    }

    // iterate over IOs to write their calibration
    void writeEeprom()
    {
      int address = 0;
      for (int i = 0; i < mIoCount; i++)
      {
        IOStruct *ioStruct = mIoList[i];
        Iio *io = ioStruct->io;
        address += io->writeCalibration(address);
      }

      EEPROM.commit();
    }

    struct IOStruct {
      Iio * io = nullptr;
      Multiplexer * multiplexer = nullptr;
      int multiplexerChannel = -1;
    };

    MIDIusb mMidi;

    // sending device id (up to 127?)
    uint8_t mController = 1;

    // midi channel ( usually stays at 0 )
    uint8_t mChannel = 0;

    int mIoCount = 0;
    int mMaxIos = 127;

    // each io can have a multiplexer assigned
    IOStruct ** mIoList;

    // buffer messages
    uint8_t mCurrentMessage = 0U;
    uint8_t mWrittenMessage = 0U;
    uint8_t mMaxMessageBuffer = 64;
    uint8_t * mMidiController = new uint8_t[mMaxMessageBuffer];
    uint8_t *mMidiValues = new uint8_t[mMaxMessageBuffer];
};
#endif