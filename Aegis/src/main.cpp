#include <Arduino.h>
#include "Omni.h"
#include "BohleBots_BNO055.h"
#include "Wire.h"
#include "camera/OpenMVStream.h"

#define LINESERIAL Serial8
OpenMVStream front(Serial2);
OpenMVStream right(Serial1);
OpenMVStream left(Serial4);
OpenMVStream back(Serial5);

Omni Drive;
BNO bno;

void setup() {
  front.begin(115200);
  back.begin(115200);
  LINESERIAL.begin(115200);
  Serial.begin(115200);
  Wire2.begin();  
  Wire2.setClock(400000);
  delay(2000);
  bno.startBNO(200,false);
  bno.saveOffsets(100);
  pinMode(13,OUTPUT);
}

void loop() {  
  digitalWrite(13,HIGH);
  if (back.update()){
    Serial.println(back.goalX);
  }
  if(front.update()){
    Serial.println(front.ballX);
  }
  else{
    Serial.print("ayuda");
    delay(100);
  }
  /*Serial.println("moviendo a  90")
  Drive.move(90,200,0);
  delay(3000);
  Serial.println("Moviendo a 180");
  Drive.move(180,200,0);
  delay(3000);
  Serial.println("Moviendo a 270");
  Drive.move(270,200,0);
  delay(3000);
  Serial.println("Moviendo a 0");
  Drive.move(0,200,0);
  delay(3000);*/
  Serial.println(bno.getHeadingAuto(100), DEC); 
  Drive.move(270,180,0);
}
