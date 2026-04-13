/**
 * @file Motors.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
  public:
    Motor(int pwmA, int pwmB);
    void move(int pot);

  private:
    int pwmA;
    int pwmB;
};

#endif