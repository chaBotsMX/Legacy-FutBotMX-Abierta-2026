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
  if(front.ballX != BALL_OUT_OF_RANGE && front.ballY != BALL_OUT_OF_RANGE){
    return 0;
  }
  else if(back.ballX != BALL_OUT_OF_RANGE && back.ballY != BALL_OUT_OF_RANGE){
    return 1;
  }
  else if(left.ballX != BALL_OUT_OF_RANGE && left.ballY != BALL_OUT_OF_RANGE){
    return 2;
  }
  else if(right.ballX != BALL_OUT_OF_RANGE && right.ballY != BALL_OUT_OF_RANGE){
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
  
  return BALL_OUT_OF_RANGE;

}

int CameraInstance::calcBallDistance() {
  if (whereIsBall() != -1) {
    switch (whereIsBall()) {
      case 0:
        return getBallDistance(front.ballX, front.ballY, 0);
      case 1:
        return getBallDistance(back.ballX, back.ballY, 0);
      case 2:
        return getBallDistance(left.ballX, left.ballY, 1);
      case 3:
        return getBallDistance(right.ballX, right.ballY, 1);
    }
  }
  return BALL_OUT_OF_RANGE;
}


void CameraInstance::kick(int trigger) {
  static elapsedMillis kickTimer = 0;
  static bool kicking = false;

  if (trigger == 1 && !kicking) {
    kickTimer = 0;
    kicking = true;
    digitalWrite(KICK, HIGH);
  }

  if (kicking && kickTimer >= 10) {
    digitalWrite(KICK, LOW);
    kicking = false;
  }
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
  int whereball = whereIsBall();
  if(whereball == -1){
    whereball = 100;
  }
  uint8_t ballAngle = calcRawBallAngle() / 2;
  uint8_t hasBall = haveBall(front.ballX, front.ballY);
  uint8_t ballDistance = calcBallDistance() / 2;
  uint8_t frontGoalX = front.goalX / 2;
  uint8_t frontGoalY = front.goalY / 2;
  uint8_t backGoalX = back.goalX / 2;
  uint8_t backGoalY = back.goalY / 2;
  uint8_t checksum = ballAngle ^ hasBall ^ ballDistance ^ frontGoalX ^ frontGoalY ^ backGoalX ^ backGoalY;

  Serial5.write(0xAA);
  Serial5.write(ballAngle);
  Serial5.write(hasBall);
  Serial5.write(ballDistance);
  Serial5.write(frontGoalX);
  Serial5.write(frontGoalY);
  Serial5.write(backGoalX);
  Serial5.write(backGoalY);
  Serial5.write(checksum);
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
  pinMode(BUTTON_PIN,INPUT);
}



void CameraInstance::update() {
  kick();
  
  OutPutHandler();
  updateCameras();
  Serial.print("ballAngle=");
  Serial.print(calcRawBallAngle());
  Serial.print(" distance=");
  Serial.print(calcBallDistance());
  Serial.print(" front.goalX=");
  Serial.print(front.goalX);
  Serial.print(" front.goalY=");
  Serial.print(front.goalY);
  Serial.print(" back.goalX=");
  Serial.print(back.goalX);
  Serial.print(" back.goalY=");
  Serial.println(back.goalY);
  Serial.print("ballX: ");
  Serial.print(front.ballX);
  Serial.print(" bally: ");
  Serial.print(front.ballY);
  Serial.print( "tiene pelota: ");
  Serial.println(haveBall(front.ballX,front.ballY));

  sendInfoToMain();
}


void CameraInstance::updateOpemMVCamData(CameraData &camData, OpenMVStream &camStream, elapsedMillis &camTimer){
  if (camStream.update()) {
    digitalWrite(LED_PIN, HIGH);
    camData.ballX = camStream.ballX;
    camData.ballY = camStream.ballY;
    camData.goalX = camStream.goalX;
    camData.goalY = camStream.goalY;
    camData.goalColor = camStream.goalColor;
    camTimer = 0;
  }
  else if (camTimer > BALL_OUT_OF_RANGE) {
    digitalWrite(LED_PIN, LOW);
    camData.ballX = BALL_OUT_OF_RANGE;
    camData.ballY = BALL_OUT_OF_RANGE;
    camData.goalX = BALL_OUT_OF_RANGE;
    camData.goalY = BALL_OUT_OF_RANGE;
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
    camData.goalX = BALL_OUT_OF_RANGE;
    camData.goalY = BALL_OUT_OF_RANGE;
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
        kick(1);
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
