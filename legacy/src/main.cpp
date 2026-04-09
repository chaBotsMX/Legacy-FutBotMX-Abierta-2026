//La pcb superior que manda anuglo esta conectada al Serial3
//la pcb de linea al seral4
#include <BohleBots_BNO055.h>
#include <Wire.h>
#include "libs/Motors.h"
BNO bno;
Motors robot;


int ballAng = 500;


int lineAng = 500;
void setup()
{  
  delay(2000);
  pinMode(13,OUTPUT);
  pinMode(1,INPUT);

  Wire.begin(); 
  Wire.setClock(400000); 
  Serial.begin(115200); 
  Serial4.begin(2000000);
  Serial3.begin(115200);
  bno.startBNO(200, false); 
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100); 
  delay(1000);
  pinMode(0,INPUT);
 
}

void loop() 
{
  int error = bno.getRLHeadingAuto(100);
  while(Serial3.available()){
    int a = Serial3.read();
    ballAng = a*2;
    Serial.println(ballAng);
  }
  if(digitalRead(0)){
    bno.setReference();
  }

  while(Serial4.available()){
    lineAng = Serial4.read() * 2;
    digitalWrite(13,HIGH);
  }
  //Serial.println(error, DEC); 
  //Serial.println(digitalRead(1)); 
  if(digitalRead(1)){
  //Serial.println(error);
    if(lineAng != 500){
      robot.omni(lineAng,200,error);
    }
    else{
      ballAng != 500 ?  robot.omni(ballAng,200,error): robot.omni(180,200,error );
    }
  }
  else{
  robot.omni(0,0,0);
  }
}
