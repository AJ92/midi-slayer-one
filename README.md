# Setup

## Dependencies
- Arduino IDE 2.x, lower than 3.x !!!
- following aditional board manager url added to preferences:
  -- https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
- esp32 board manager in version 2.0.4 (3.x or higher not supported, hal/usb_hal.h error!!!)
- https://github.com/chegewara/EspTinyUSB.git  yes the old one works...
- board: esp32 S2 dev module works as target

## Notes:
- default web credentials user: admin, password: admin
- to enter OTA firmware flasher: hold upper button during power up
  ```
  // wifisecrets.h under midictrl
  #define SECRET_SSID "SSID"
  #define SECRET_PASS "PW"
  ```
- to enter potentiometer calibration: hold upper right button during power up
  - release button, move potentiometers to min, then to max, then center them
  - press upper right button again to store eeprom