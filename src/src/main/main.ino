//La pcb superior que manda anuglo esta conectada al Serial3
//la pcb de linea al seral4
#include <BohleBots_BNO055.h>
#include <Wire.h>
#include <Motors.h>
BNO bno;
Motors robot;


int ballAng = 500;



void setup()
{  
  delay(2000);
  pinMode(13,OUTPUT);
  pinMode(1,INPUT);

  Wire.begin(); 
  Wire.setClock(400000); 
  Serial.begin(115200); 
  Serial4.begin(115200);
  Serial3.begin(115200);
  bno.startBNO(200, false); 
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100); 
  delay(1000);
  pinMode(0,INPUT);
  digitalWrite(13,HIGH);
}

void loop() 
{
  int error = bno.getRLHeadingAuto(100);
  if(Serial3.available()){
    int a = Serial3.read();
    ballAng = a*2;
  }
  if(digitalRead(0)){
    bno.setReference();
  }


  //Serial.println(error, DEC); 
  //Serial.println(digitalRead(1)); 
  if(digitalRead(1)){
  Serial.println(error);
  ballAng != 500 ?  robot.omni(ballAng,200,error): robot.omni(180,200,error );
  
    
  }
  else{
  robot.omni(0,0,0);
  }
}

