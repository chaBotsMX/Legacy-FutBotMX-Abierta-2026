//La pcb superior que manda anuglo esta conectada al Serial3
//la pcb de linea al seral4
#include <BohleBots_BNO055.h>
#include <Wire.h>
#include "Omni.h"
BNO bno;
Omni robot;


int ballAng = 500;

int lineAng = 500;

static int correctLineAngle(int angle) {
  // Ajusta el ángulo recibido al marco de referencia del robot.
  return (angle + 90) % 360;
}

void setup()
{  
  delay(2000);
  pinMode(13,OUTPUT);
  pinMode(1,INPUT);

  Wire.begin(); 
  Wire.setClock(400000); 
  Serial.begin(115200); 
  Serial4.begin(2000000);
  Serial3.begin(2000000);
  bno.startBNO(200, false); 
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100); 
  delay(1000);
  pinMode(0,INPUT);
 
}

void loop() 
{
  
  int error = bno.getRLHeadingAuto(100);
  /*
  while(Serial3.available()){
    int a = Serial3.read();
    ballAng = a*2;
    Serial.println(ballAng);
  }
  */
  if(digitalRead(0)){
    bno.setReference();
  }

  while(Serial3.available()){
    int rawAngle = Serial3.read() * 2;
    if(rawAngle != 500){
      lineAng = correctLineAngle(rawAngle);
    }
    else{
      lineAng = 500;
    }
    digitalWrite(13,HIGH);
    Serial.println(lineAng);
  }
  //Serial.println(error, DEC); 
  //Serial.println(digitalRead(1)); 
  if(digitalRead(1)){
  //Serial.println(error);
    if(lineAng != 500){
      robot.move(lineAng,200,error);
    }
    else{
      ballAng != 500 ?  robot.move(ballAng,200,error): robot.move(180,200,error );
    }
  }
  else{
  robot.move(0,0,0);
  }
}
