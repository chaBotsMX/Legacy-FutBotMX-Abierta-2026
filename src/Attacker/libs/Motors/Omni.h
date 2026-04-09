/**
 * @file Omni.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#ifndef OMNI_H
#define OMNI_H

#include "Motor.h"
#include <Arduino.h>

#define NUM_MOTORS 4

#define MOTOR_1_A 5
#define MOTOR_1_B 4

#define MOTOR_2_A 3
#define MOTOR_2_B 2

#define MOTOR_3_A 11
#define MOTOR_3_B 10

#define MOTOR_4_A 9
#define MOTOR_4_B 8

#define MOTOR_ANGLE_OFFSET 45

#define BALL_OUT_OF_RANGE 500

#define PI acos((long double)-1);

class Omni {
  public:
    Omni();

    void motorMove(int mot, int pot);
    void move(int angle, int power,int rotation);

  private:
    int prevRot = -1;
    Motor motors[NUM_MOTORS] = {
        Motor(MOTOR_1_A, MOTOR_1_B),
        Motor(MOTOR_2_A, MOTOR_2_B),
        Motor(MOTOR_3_A, MOTOR_3_B),
        Motor(MOTOR_4_A, MOTOR_4_B),
    };
};

#endif