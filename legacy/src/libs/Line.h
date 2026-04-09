/**
 * @file Line.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
 */

#ifndef Line_H
#define Line_H


#define numSensors 16
#define bufferSize 60

#include <Arduino.h>

class Line {

  public:
    Line(int _threshold = 300);
    void calculateDepth();
    void calculateSide();
    int getLineSector(float lineAngle);
    int lineSwitch(int sector, int lastSector);
    int adjustLineAngle(int angle);
    int getAvoidAngle();
    void update();

  private:
    int analogs[15] = {17,18,19,20,24,25,26,27,41,40,39,38,14,15,16};
    // 0, 22.5, 45, 67.5, 90...
    float cosenos[15] = {1,0.9238,0.7071,0.3826,0,-0.3826,-0.7071,-0.9238,-0.9238,-0.7071,-0.3826,0,0.3826,0.7071,0.9238};
    float senos[15]   = {0,0.3826,0.7071,0.9238,1,0.9238,0.7071,0.3826,-0.3826,-0.7071,-0.9238,-1,-0.9238,-0.7071,-0.3826};
    bool detectedSensors[15];
    bool firstDetected = false;
    int firstSector = -1;
    int sector = -1;
    int fixedLineAngle = 500;
    int avoidAngle = 500;
    int threshold = 300;
    float angle = 500;

    };

#endif