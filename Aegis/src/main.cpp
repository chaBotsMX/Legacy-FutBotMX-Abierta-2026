#include <Arduino.h>
#include "Omni.h"
#include "BohleBots_BNO055.h"
#include "Wire.h"
#include "camera/OpenMVStream.h"
#include "utils.h"

#define LINESERIAL Serial8

OpenMVStream front(Serial2);
OpenMVStream back(Serial1);



Omni Drive;
BNO bno;
int setPointGoalY = 224;
bool play = 0;
int setPoint = 0;

int calcOrientationError(){
  return wrap180(setPoint - bno.getHeadingAuto(100));
  
}
void setup() {
  front.begin(115200);
  back.begin(115200);
  Serial.begin(115200);
  Wire2.begin();  
  Wire2.setClock(400000);
  delay(2000);
  bno.startBNO(200,false);
  bno.saveOffsets(100);
  pinMode(13,OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

}

void loop() {  
  digitalWrite(13,HIGH);
  if(back.update()){

  }
  if(digitalRead(4)){
    setPoint = bno.getHeadingAuto(100);
    if(play == 0){
      play = 1;
    }
    else{
      play = 0;
    }
  }
  int goalYError = 500;
  if(back.goalY != 500){
    goalYError = back.goalY - setPointGoalY;  
  }
  //Serial.println(goalYError);
  Serial.println(calcOrientationError());
  if(play){
    if(goalYError == 500){
      Drive.move(180,160,calcOrientationError());
    }
    else if(goalYError > 6){
      Drive.move(180,160,calcOrientationError());
    }
    else if(goalYError < -6){
      Drive.move(0,160,calcOrientationError());
    }
    else{
      Drive.move(500,0,calcOrientationError());
    }
  }
}
