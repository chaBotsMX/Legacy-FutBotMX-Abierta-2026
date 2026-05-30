#include "camera/OpenMVStream.h"
#include "camera/UnitVStream.h"
#include "uitls.h" 
#include "ballProccesing.h"
#define BALL_OUT_OF_RANGE 500
#define CAMERA_RESOLUTION_WIDTH 320
#define CAMERA_RESOLUTION_HEIGHT 240

#define LED_PIN 13
#define KICK_PIN 2
#define DRIBBLER1_PIN 3
#define DRIBBLER2_PIN 4
elapsedMillis frontCamTimer = 0;
elapsedMillis backCamTimer = 0;
elapsedMillis leftCamTimer = 0;
elapsedMillis rightCamTimer = 0;


OpenMVStream FrontCam(Serial4);
OpenMVStream BackCam(Serial3);
UnitVStream LeftCam(Serial1);
UnitVStream RightCam(Serial2);

int ballAng = 0;
int ballFrontX, ballFrontY, goalFrontX, goalFrontY, goalFrontColor;
int ballBackX, ballBackY, goalBackX, goalBackY, goalBackColor;
int ballRigthX, ballRigthY;
int ballLeftX, ballLeftY;


void kick(){
  digitalWrite(KICK_PIN, HIGH);
  delay(10);
  digitalWrite(KICK_PIN, LOW);
}
int calcRawBallAngle() {
  int resultAng = 500;
  if (ballFrontX != BALL_OUT_OF_RANGE) {
    return getBallAngle(ballFrontX, ballFrontY, 0); 
  }
 /* else if(ballBackX != BALL_OUT_OF_RANGE){
    return getBallAngle(ballBackX, ballBackY, 1);
  }*/
  else if(ballLeftX != BALL_OUT_OF_RANGE){
    return getBallAngle(ballLeftX, ballLeftY, 2);
  }
  else if(ballRigthX != BALL_OUT_OF_RANGE){
    return getBallAngle(ballRigthX, ballRigthY, 3);
  }
  else{
    return resultAng;
  }

}



void sendInfoToMain(){
  Serial5.write(0xAA);
  Serial5.write(ballAng / 2);
  Serial5.write(haveBall(ballFrontX, ballFrontY));
  Serial5.write(getGoalAngle(goalFrontX,goalBackY) / 2);
  Serial5.write(0x55);
}


void turnOnDribbler(){
  digitalWrite(DRIBBLER1_PIN, HIGH);
  digitalWrite(DRIBBLER2_PIN, LOW);
}
void turnOffDribbler(){
  digitalWrite(DRIBBLER1_PIN, LOW);
  digitalWrite(DRIBBLER2_PIN, LOW);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial5.begin(115200);

  FrontCam.begin();
  BackCam.begin();
  LeftCam.begin();
  RightCam.begin();
  pinMode(KICK_PIN, OUTPUT);
  pinMode(DRIBBLER1_PIN, OUTPUT);
  pinMode(DRIBBLER2_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}



void loop() {
  if(Serial5.available()){
    int command = Serial5.read();
    if(command == 1){
      kick();
    }
    else if(command == 2){
      turnOnDribbler();
    }
    else if(command == 3){
      turnOffDribbler();
    }
  }
  if (FrontCam.update()) {
    ballFrontX = FrontCam.ballX;
    ballFrontY = FrontCam.ballY;
    goalFrontX = FrontCam.goalX;
    goalFrontY = FrontCam.goalY;
    goalFrontColor = FrontCam.goalColor;
    frontCamTimer = 0;
  }
  else if (frontCamTimer > 500) {
    ballFrontX = BALL_OUT_OF_RANGE;
    ballFrontY = BALL_OUT_OF_RANGE;
  }

  if (BackCam.update()) {
    ballBackX = BackCam.ballX;
    ballBackY = BackCam.ballY;
    goalBackX = BackCam.goalX;
    goalBackY = BackCam.goalY;
    goalBackColor = BackCam.goalColor;
    backCamTimer = 0;
  }
  else if (backCamTimer > 500) {
    ballBackX = BALL_OUT_OF_RANGE;
    ballBackY = BALL_OUT_OF_RANGE;
  }

  if (LeftCam.update()) {
    ballLeftX = LeftCam.ballX;
    ballLeftY = LeftCam.ballY;
    leftCamTimer = 0;
  }
  else if (leftCamTimer > 500) {
    ballLeftX = BALL_OUT_OF_RANGE;
    ballLeftY = BALL_OUT_OF_RANGE;
  }
  if (RightCam.update()) {
    ballRigthX = RightCam.ballX;
    ballRigthY = RightCam.ballY;
    rightCamTimer = 0;
  }
  else if (rightCamTimer > 500) {
    ballRigthX = BALL_OUT_OF_RANGE;
    ballRigthY = BALL_OUT_OF_RANGE;
  }
 

  ballAng = calcRawBallAngle();
  sendInfoToMain();
}


