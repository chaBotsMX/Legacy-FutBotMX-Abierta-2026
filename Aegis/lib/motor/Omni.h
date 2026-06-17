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

#define MOTOR_1_A 37
#define MOTOR_1_B 36

#define MOTOR_2_A 15
#define MOTOR_2_B 14


// ok
#define MOTOR_3_A 12
#define MOTOR_3_B 11

// 4 ok
#define MOTOR_4_A 9
#define MOTOR_4_B 10

#define MOTOR_ANGLE_OFFSET 45

#define BALL_OUT_OF_RANGE 500


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