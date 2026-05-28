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
#include <Adafruit_NeoPixel.h>

#define STARTUP_DELAY 1000

#define NUM_SENSORS 32
#define BUFFER_SIZE 60
#define NUM_SECTORS 12

#define LINE_SENSOR_ANGLE_STEP 11.25
#define BALL_OUT_OF_RANGE 500

#define LINE_THRESHOLD 300

#define LOWER_ANGLE_BOUND -15
#define UPPER_ANGLE_BOUND 345

#define SECTOR_ANGLE_STEP 30
#define SECTOR_OFFSET 15

#define LED_PIN 10

#define PI acos((long double)-1)

class Line {

  public:
    Line();

    void calculateDepth();
    void calculateSide();
    int getLineSector(float lineAngle);
    int lineSwitch(int sector, int lastSector);
    int adjustLineAngle(int angle);
    int getAvoidAngle() { return avoidAngle; }

    void update();
    void generateVoltages();

  private:
    Adafruit_NeoPixel pixels;
    // Pines de sensores ordenados en sentido de las manecillas del reloj
    int analogs[NUM_SENSORS] = {36, 29, 30, 31, 32, 27, 26, 25, 24, 9, 8, 7, 6, 0, 1, 2, 3, 20, 21, 22, 23, 19, 18, 17, 16, 40, 39, 38, 37, 33, 34, 35};

    // Pines usados para generar voltajes de referencia en el filtro RC
    const int vref[4] = {4, 12, 13, 28};

    // Senos y cosenos para angulos de 0 a 360 cada 11.25 grados
    // 0, 11.25, 22.5, 33.75, 45...
    float cosenos[NUM_SENSORS];
    float senos[NUM_SENSORS];

    bool detectedSensors[NUM_SENSORS];
    bool firstDetected = false;
    int firstSectorDetected = -1;
    int sector = -1;
    int fixedLineAngle = BALL_OUT_OF_RANGE;
    int avoidAngle = BALL_OUT_OF_RANGE;
    float angle = BALL_OUT_OF_RANGE;
    int threshold = LINE_THRESHOLD;

    void calculateSinAndCos();
    int filterAngle(int angle);
};

#endif