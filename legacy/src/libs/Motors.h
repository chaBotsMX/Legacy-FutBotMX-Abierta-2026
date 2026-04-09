/**
 * @file Motors.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#ifndef Motors_H
#define Motors_H

#define numSensors 16
#define bufferSize 60

#include <Arduino.h>

class Motors {
  public:
    Motors();
    void motorMove(int mot, int pot);
    void omni(int angle, int power,int rotation);
  private:
    int motor[4][2] = {{5,4},{3,2},{11,10},{9,8}};
    int prevRot;
};

#endif