/**
 * midi-slayer-one
 * author: vtx-slayer
 */
#include "controller.h"
#include "btn.h"
#include "pot.h"
#include "rotenc.h"

Controller controller;
Btn b1(4);
Pot p1(4);

RotEnc rotEnc(10, 11, 12);

Multiplexer mp(4,5,6,7,8,9);

void setup() {
  Serial.begin(115200);

  controller.addIo(&b1, &mp, 0);
  controller.addIo(&p1, &mp, 1);

  controller.addIo(&rotEnc.getButton());
  controller.addIo(&rotEnc.getSwitchLeft());
  controller.addIo(&rotEnc.getSwitchRight());
}

void loop() {
  controller.loop();
}
