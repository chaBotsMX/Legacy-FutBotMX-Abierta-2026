/**
 * @file Line.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
 */

#ifndef Line_H
#define Line_H

#include <Arduino.h>
#include <cmath>

#define STARTUP_DELAY 1000

#define NUM_SENSORS 15
#define BUFFER_SIZE 60
#define NUM_SECTORS 12

#define LINE_SENSOR_ANGLE_STEP 22.5
#define BALL_OUT_OF_RANGE 500

#define LINE_THRESHOLD 300

#define LOWER_ANGLE_BOUND -15
#define UPPER_ANGLE_BOUND 345

#define SECTOR_ANGLE_STEP 30
#define SECTOR_OFFSET 15

#define LED_PIN 13

#define PI acos((long double)-1)

class Line {

  public:
    Line(int threshold);

    void calculateDepth();
    void calculateSide();
    int getLineSector(float lineAngle);
    int lineSwitch(int sector, int lastSector);
    int adjustLineAngle(int angle);
    int getAvoidAngle() { return avoidAngle; }

    void update();

  private:
    // Pines de sensores ordenados en sentido de las manecillas del reloj
    int analogs[NUM_SENSORS] = {17,18,19,20,24,25,26,27,41,40,39,38,14,15,16};

    // Senos y cosenos para angulos de 0 a 360 cada 22.5 grados
    // 0, 22.5, 45, 67.5, 90...
    float cosenos[NUM_SENSORS];
    float senos[NUM_SENSORS];

    bool detectedSensors[NUM_SENSORS];
    bool firstDetected = false;
    int firstSectorDetected = -1;
    int sector = -1;
    int fixedLineAngle = BALL_OUT_OF_RANGE;
    int avoidAngle = BALL_OUT_OF_RANGE;
    float angle = BALL_OUT_OF_RANGE;
    int threshold;

    void calculateSinAndCos();
    int filterAngle(int angle);
};

#endif