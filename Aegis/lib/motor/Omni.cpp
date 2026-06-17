/**
 * @file Omni.cpp
 * @brief Methods to control the motors
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#include "Omni.h"

Omni::Omni() {}

void Omni::move(int angle, int power,int rotation){
  if(angle == BALL_OUT_OF_RANGE){
    float output = constrain((rotation * 12), -165, 165) + (((rotation - prevRot) / 2) * 64);
    prevRot = rotation;
    for(auto &motor : motors) motor.move(int(output));
  }
  else{
    float A = power * cos((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);
    float B = -power * sin((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);
    float C = power * sin((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);
    float D = -power * cos((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);
/*
    Serial.print(A); Serial.print('\t');
    Serial.print(B); Serial.print('\t');
    Serial.print(C); Serial.print('\t');
    Serial.print(D); Serial.print('\t');
    Serial.println("");
*/
    float maxVal = max(max(abs(A), abs(B)), max(abs(C), abs(D))); //Serial.print(maxVal); Serial.print('\t');
    if (maxVal > 0) {
      float scale = float(power) / maxVal;
      //Serial.print(scale); Serial.print('\n');
      A = A * scale;
      B = B * scale;
      C = C * scale;
      D = D * scale;
    }

    motors[0].move(int(A + rotation)); //back right
    motors[1].move(int(B + rotation)); //front right
    motors[2].move(int(C + rotation)); //front left
    motors[3].move(int(D + rotation)); //back left
  }
}
