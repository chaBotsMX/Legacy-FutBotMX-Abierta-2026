/**
 * @file Motors.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#ifndef Motors_H
#define Motors_H

#include <Arduino.h>

#define NUM_MOTORS 4
#define NUM_PINS 2

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

class Motors {
  public:
    Motors();

    void motorMove(int mot, int pot);
    void omni(int angle, int power,int rotation);

  private:
    int motors[NUM_MOTORS][NUM_PINS] = {
      {MOTOR_1_A, MOTOR_1_B},
      {MOTOR_2_A, MOTOR_2_B},
      {MOTOR_3_A, MOTOR_3_B},
      {MOTOR_4_A, MOTOR_4_B},
    };
};

#endif