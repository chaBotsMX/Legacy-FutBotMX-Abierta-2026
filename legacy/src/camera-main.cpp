#include "camera/OpenMVStream.h"
#include "camera/UnitVStream.h"

#define BALL_OUT_OF_RANGE 500
#define ITERATION_DELAY 1000
#define CAMERA_RESOLUTION_WIDTH 320

#define LED_PIN 13

OpenMVStream FrontCam(Serial4);
OpenMVStream BackCam(Serial2);
UnitVStream LeftCam(Serial3);
UnitVStream RightCam(Serial1);

int ballAng = 0;
int ballFrontX, ballFrontY, goalFrontX, goalFrontY, goalFrontColor;
int ballBackX, ballBackY, goalBackX, goalBackY, goalBackColor;
int ballRigthX, ballRigthY;
int ballLeftX, ballLeftY;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial5.begin(115200);

  FrontCam.begin();
  BackCam.begin();
  LeftCam.begin();
  RightCam.begin();

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (FrontCam.update()) {
    FrontCam.getBallX(ballFrontX);
    ballFrontX -= CAMERA_RESOLUTION_WIDTH / 2;
  }
  if (BackCam.updatePacketOpenMV(ballBackX, ballBackY, goalBackX, goalBackY,
                                 goalBackColor)) {
    Serial.print(" camBack Working");
  }
  if (LeftCam.updatePacketUnitV(ballLeftX, ballLeftY)) {
    Serial.print(" camLeft Working");
  }
  if (RightCam.updatePacketUnitV(ballRigthX, ballRigthY)) {
    Serial.println(" camRigth Working");
  }
  Serial5.write(ballAng / 2);
  delay(ITERATION_DELAY);
}
