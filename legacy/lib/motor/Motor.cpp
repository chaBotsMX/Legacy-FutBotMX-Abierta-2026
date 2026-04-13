/**
 * @file Motor.cpp
 * @brief Methods to control the motors
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#include "Motor.h"

Motor::Motor(int pwmA, int pwmB) : pwmA(pwmA), pwmB(pwmB) {
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB,OUTPUT);
  analogWriteFrequency(pwmA, 100000);
  analogWriteFrequency(pwmB, 100000);
}

void Motor::move(int pot){
  if(pot > 0){
    analogWrite(pwmA, pot);
    analogWrite(pwmB, 0);
  }
  else {
    analogWrite(pwmA, 0);
    analogWrite(pwmB, abs(pot));
  }
}