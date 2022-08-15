/**
 * midi-slayer-one
 * author: vtx-slayer
 */
#include "controller.h"
#include "btn.h"
#include "pot.h"
#include "rotenc.h"
#include "led.h"
#include "ota.h"

Controller controller;

Multiplexer mp0(5,0 /*unused*/,1,2,3,4);
Multiplexer mp1(6,0 /*unused*/,1,2,3,4);
Multiplexer mp2(7,0 /*unused*/,1,2,3,4);
Multiplexer mp3(8,0 /*unused*/,1,2,3,4);

// mp 0
Btn b0(5); //@ 0
Btn b1(5); //@ 1

Pot p0(5); //@ 2
Pot p1(5); //@ 3

Btn b2(5); //@ 4
Btn b3(5); //@ 5
Btn b4(5); //@ 6
Btn b5(5); //@ 7
Btn b6(5); //@ 8
Btn b7(5); //@ 9
Btn b8(5); //@ 10
Btn b9(5); //@ 11
Btn b10(5); //@ 12
Btn b11(5); //@ 13

Pot p2(5); //@ 14
Pot p3(5); //@ 15

//mp 2
Btn b12(7); //@ 0
Btn b13(7); //@ 1

Pot p4(7); //@ 2
Pot p5(7); //@ 3

Btn b14(7); //@ 4
Btn b15(7); //@ 5
Btn b16(7); //@ 6
Btn b17(7); //@ 7
Btn b18(7); //@ 8
Btn b19(7); //@ 9
Btn b20(7); //@ 10
Btn b21(7); //@ 11
Btn b22(7); //@ 12
Btn b23(7); //@ 13

Pot p6(7); //@ 14
Pot p7(7); //@ 15

//mp 1
Led led0(6); //@ 0
Led led1(6); //@ 1
Led led2(6); //@ 2
Led led3(6); //@ 3
Led led4(6); //@ 4
Led led5(6); //@ 5

Btn b24(6); //@ 6

Pot p8(6); //@ 7  linear
Pot p9(6); //@ 8  linear
Pot p10(6); //@ 9
Pot p11(6); //@ 10
Pot p12(6); //@ 11
Pot p13(6); //@ 12
Pot p14(6); //@ 13

//mp 3
Led led6(8); //@ 1
Led led7(8); //@ 2
Led led8(8); //@ 3
Led led9(8); //@ 4
Led led10(8); //@ 5
Led led11(8); //@ 6

Btn b25(8); //@ 7

Pot p15(8); //@ 8  linear
Pot p16(8); //@ 9
Pot p17(8); //@ 10
Pot p18(8); //@ 11
Pot p19(8); //@ 12
Pot p20(8); //@ 13

RotEnc rotEnc1(9, 10, 11, 0);
RotEnc rotEnc2(12, 13, 16, 1);

bool ota = false;


//Pot test1(5); //@ 5
//Pot test2(5); //@ 14


void setup() {
  Serial.begin(115200);

  delay(1000);

  Serial.println("midi-slayer-one setup...");

  //controller.addIo(&test1, &mp0, 5);
  //controller.addIo(&test2, &mp0, 14);

  // mp 0
  controller.addIo(&b0, &mp0, 0);
  controller.addIo(&b1, &mp0, 1);

  controller.addIo(&p0, &mp0, 2);
  controller.addIo(&p1, &mp0, 3);

  controller.addIo(&b2, &mp0, 4);
  controller.addIo(&b3, &mp0, 5);
  controller.addIo(&b4, &mp0, 6);
  controller.addIo(&b5, &mp0, 7);
  controller.addIo(&b6, &mp0, 8);
  controller.addIo(&b7, &mp0, 9);
  controller.addIo(&b8, &mp0, 10);
  controller.addIo(&b9, &mp0, 11);
  controller.addIo(&b10, &mp0, 12);
  controller.addIo(&b11, &mp0, 13);

  controller.addIo(&p2, &mp0, 14);
  controller.addIo(&p3, &mp0, 15);

  // mp 2 ( equal to mp 0, but on the other side, and flipped channels )
  controller.addIo(&b12, &mp2, 15);
  controller.addIo(&b13, &mp2, 14);

  controller.addIo(&p4, &mp2, 13);
  controller.addIo(&p5, &mp2, 12);

  controller.addIo(&b14, &mp2, 11);
  controller.addIo(&b15, &mp2, 10);
  controller.addIo(&b16, &mp2, 9);
  controller.addIo(&b17, &mp2, 8);
  controller.addIo(&b18, &mp2, 7);
  controller.addIo(&b19, &mp2, 6);
  controller.addIo(&b20, &mp2, 5);
  controller.addIo(&b21, &mp2, 4);
  controller.addIo(&b22, &mp2, 3);
  controller.addIo(&b23, &mp2, 2);

  controller.addIo(&p6, &mp2, 1);
  controller.addIo(&p7, &mp2, 0);

  // mp 1
  controller.addIo(&led0, &mp1, 0);
  controller.addIo(&led1, &mp1, 1);
  controller.addIo(&led2, &mp1, 2);
  controller.addIo(&led3, &mp1, 3);
  controller.addIo(&led4, &mp1, 4);
  controller.addIo(&led5, &mp1, 5);

  controller.addIo(&b24, &mp1, 6);

  controller.addIo(&p8, &mp1, 7);
  controller.addIo(&p9, &mp1, 8);
  controller.addIo(&p10, &mp1, 9);
  controller.addIo(&p11, &mp1, 10);
  controller.addIo(&p12, &mp1, 11);
  controller.addIo(&p13, &mp1, 12);
  controller.addIo(&p14, &mp1, 13);

  // mp 3 (equal to mp, but first linear pot is missing and prior ios are moved by one )
  controller.addIo(&led6, &mp3, 1);
  controller.addIo(&led7, &mp3, 2);
  controller.addIo(&led8, &mp3, 3);
  controller.addIo(&led9, &mp3, 4);
  controller.addIo(&led10, &mp3, 5);
  controller.addIo(&led11, &mp3, 6);

  controller.addIo(&b25, &mp3, 7);

  controller.addIo(&p15, &mp3, 8);
  controller.addIo(&p16, &mp3, 9);
  controller.addIo(&p17, &mp3, 10);
  controller.addIo(&p18, &mp3, 11);
  controller.addIo(&p19, &mp3, 12);
  controller.addIo(&p20, &mp3, 13);


  // rot encs
  controller.addIo(&rotEnc1.getButton());
  controller.addIo(&rotEnc1.getSwitchLeft());
  controller.addIo(&rotEnc1.getSwitchRight());

  controller.addIo(&rotEnc2.getButton());
  controller.addIo(&rotEnc2.getSwitchLeft());
  controller.addIo(&rotEnc2.getSwitchRight());

  //if b0 is pressed at start go into wifi ota mode
  mp0.setChannel(0);
  delay(1);
  b0.loop();
  if(b0.isPressed()){
    Serial.println("holding b0 .");
  }
  delay(100);
  mp0.setChannel(0);
  b0.loop();
  if(b0.isPressed()){
    Serial.println("holding b0 ..");
  }

  if(b0.isPressed()){
    Serial.println("Entering OTA");
    ota = true;
    setupOTA();
    setLeds(255);
  }
  else{
    setLeds(0);
  }
}

void loop() {
  if(ota){
    handleOTA();
  }
  controller.loop();
}

void setLeds(int val){
  led0.setValue(val);
  led1.setValue(val);
  led2.setValue(val);
  led3.setValue(val);
  led4.setValue(val);
  led5.setValue(val);

  led6.setValue(val);
  led7.setValue(val);
  led8.setValue(val);
  led9.setValue(val);
  led10.setValue(val);
  led11.setValue(val);
}