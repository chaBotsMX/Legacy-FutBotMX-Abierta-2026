#include "Cams.h"


Cams FrontCam(Serial4);
Cams BackCam(Serial2);
Cams LeftCam(Serial3);
Cams RightCam(Serial1);

int ballAng = 0;
int ballFrontX,ballFrontY,goalFrontX,goalFrontY,goalFrontColor;
int ballBackX, ballBackY, goalBackX,goalBackY,goalBackColor;
int ballRigthX, ballRigthY;
int ballLeftX, ballLeftY;

int RadToGrad(int )
void processData(){

}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial5.begin(115200);
  FrontCam.begin();
  BackCam.begin();
  LeftCam.begin();
  RightCam.begin();
  pinMode(13,OUTPUT);
}

void loop() {

  if(FrontCam.updatePacketOpenMV(ballFrontX,ballFrontY,goalFrontX,goalFrontY,goalFrontColor)){
    if(ballFrontX != 500){
      ballFrontX = ballFrontX - 160;
      
    }
  }
  if(BackCam.updatePacketOpenMV(ballBackX, ballBackY, goalBackX, goalBackY, goalBackColor)){
    Serial.print(" camBack Working");
  }
  if(LeftCam.updatePacketUnitV(ballLeftX,ballLeftY)){
    Serial.print(" camLeft Working");
  }
  if(RightCam.updatePacketUnitV(ballRigthX,ballRigthY)){
    Serial.println(" camRigth Working");
  }
  Serial5.write(ballAng/2);
  delay(1000);
}
