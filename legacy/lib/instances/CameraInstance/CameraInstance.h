#ifndef CameraInstance_H
#define CameraInstance_H

#include <Arduino.h>
#include <cmath>
#include "camera/OpenMVStream.h"
#include "camera/UnitVStream.h"
#include <utils.h>
#include "ballProccesing.h"
#include "HardWareSerial.h"

#define BALL_OUT_OF_RANGE 500
#define CAMERA_RESOLUTION_WIDTH 320
#define CAMERA_RESOLUTION_HEIGHT 240
#define KICK 2
#define DRIBBLER1 3
#define DRIBBLER2 4
#define LED_PIN 13

struct CameraData {
    int ballX = BALL_OUT_OF_RANGE;
    int ballY = BALL_OUT_OF_RANGE;
    int goalX = BALL_OUT_OF_RANGE;
    int goalY = BALL_OUT_OF_RANGE;
    int goalColor = 0;
};

extern int ballAng;

extern CameraData front;
extern CameraData back;
extern CameraData left;
extern CameraData right;

extern elapsedMillis frontCamTimer;
extern elapsedMillis backCamTimer;
extern elapsedMillis leftCamTimer;
extern elapsedMillis rightCamTimer;

extern OpenMVStream FrontCam;
extern OpenMVStream BackCam;
extern UnitVStream LeftCam;
extern UnitVStream RightCam;

class CameraInstance {
public:
    CameraInstance();
    void update();
    void init();

private:
    int whereIsBall();
    int calcRawBallAngle();
    int calcBallDistance();
    void kick();
    void turnOnDribbler();
    void turnOffDribbler();
    void sendInfoToMain();
    void OutPutHandler();
    void updateCameras();
    void updateOpemMVCamData(CameraData &camData, OpenMVStream &camStream, elapsedMillis &camTimer);
    void updateUnitVCamData(CameraData &camData, UnitVStream &camStream, elapsedMillis &camTimer);
};

#endif