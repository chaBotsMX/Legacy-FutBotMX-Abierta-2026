#include "CameraInstance.h"

int ballAng = 0;

CameraData front;
CameraData back;
CameraData left;
CameraData right;

elapsedMillis frontCamTimer = 0;
elapsedMillis backCamTimer = 0;
elapsedMillis leftCamTimer = 0;
elapsedMillis rightCamTimer = 0;

OpenMVStream FrontCam(Serial4);
OpenMVStream BackCam(Serial3);
UnitVStream LeftCam(Serial1);
UnitVStream RightCam(Serial2);


CameraInstance::CameraInstance() {
  
}


int CameraInstance::whereIsBall() {
  if(haveBall(front.ballX, front.ballY)){
    return 0;
  }
  /*else if(haveBall(back.ballX, back.ballY)){
    return 1;
  }*/
  else if(haveBall(left.ballX, left.ballY)){
    return 2;
  }
  else if(haveBall(right.ballX, right.ballY)){
    return 3;
  }
  else{
    return -1;
  }
}
int CameraInstance::calcRawBallAngle() {

  if (whereIsBall() != -1) {
    switch (whereIsBall()) {
      case 0:
        return getBallAngle(front.ballX, front.ballY, 0);
      case 1:
        return getBallAngle(back.ballX, back.ballY, 1);
      case 2:
        return getBallAngle(left.ballX, left.ballY, 2);
      case 3:
        return getBallAngle(right.ballX, right.ballY, 3);
    }
  }
  else {
    return BALL_OUT_OF_RANGE;
  }

}

int CameraInstance::calcBallDistance() {
  if (whereIsBall() != -1) {
    switch (whereIsBall()) {
      case 0:
        return getBallDistance(front.ballX, front.ballY, 0);
      case 1:
        return getBallDistance(back.ballX, back.ballY, 1);
      case 2:
        return getBallDistance(left.ballX, left.ballY, 2);
      case 3:
        return getBallDistance(right.ballX, right.ballY, 3);
    }
  }
  else {
    return BALL_OUT_OF_RANGE;
  }
}


void CameraInstance::kick(){
  digitalWrite(KICK, HIGH);
  delay(10);
  digitalWrite(KICK, LOW);
}

void CameraInstance::turnOnDribbler(){
  digitalWrite(DRIBBLER1, HIGH);
  digitalWrite(DRIBBLER2, LOW);
}

void CameraInstance::turnOffDribbler(){
  digitalWrite(DRIBBLER1, LOW);
  digitalWrite(DRIBBLER2, LOW);
}

void CameraInstance::sendInfoToMain(){
  Serial5.write(0xAA);
  Serial5.write(calcRawBallAngle() / 2);
  Serial5.write(haveBall(front.ballX, front.ballY));
  Serial5.write(front.goalX);
  Serial5.write(front.goalY);
  Serial5.write(back.goalX);
  Serial5.write(back.goalY);
  Serial5.write(0x55);
}

void CameraInstance::init() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial5.begin(2000000);

  FrontCam.begin();
  BackCam.begin();
  LeftCam.begin();
  RightCam.begin();
  pinMode(KICK, OUTPUT);
  pinMode(DRIBBLER1, OUTPUT);
  pinMode(DRIBBLER2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}



void CameraInstance::update() {
  OutPutHandler();
  updateCameras();
  sendInfoToMain();
}


void CameraInstance::updateOpemMVCamData(CameraData &camData, OpenMVStream &camStream, elapsedMillis &camTimer){
  if (camStream.update()) {
    camData.ballX = camStream.ballX;
    camData.ballY = camStream.ballY;
    camData.goalX = camStream.goalX;
    camData.goalY = camStream.goalY;
    camData.goalColor = camStream.goalColor;
    camTimer = 0;
  }
  else if (camTimer > 500) {
    camData.ballX = BALL_OUT_OF_RANGE;
    camData.ballY = BALL_OUT_OF_RANGE;
  }
}

void CameraInstance::updateUnitVCamData(CameraData &camData, UnitVStream &camStream, elapsedMillis &camTimer){
  if (camStream.update()) {
    camData.ballX = camStream.ballX;
    camData.ballY = camStream.ballY;
    camTimer = 0;
  }
  else if (camTimer > 500) {
    camData.ballX = BALL_OUT_OF_RANGE;
    camData.ballY = BALL_OUT_OF_RANGE;
  }
}

void CameraInstance::updateCameras(){
  updateOpemMVCamData(front, FrontCam, frontCamTimer);
  updateOpemMVCamData(back, BackCam, backCamTimer);
  updateUnitVCamData(left, LeftCam, leftCamTimer);
  updateUnitVCamData(right, RightCam, rightCamTimer);
}

void CameraInstance::OutPutHandler(){
  if(Serial5.available()){
    int command = Serial5.read();
    switch (command) {
      case 1:
        kick();
        break;
      case 2:
        turnOnDribbler();
        break;
      case 3:
        turnOffDribbler();
        break;
      } 
  }    
}