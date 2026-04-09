/**
 * @file Motors.cpp
 * @brief Methods to control the motors
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#include "Motors.h"

Motors::Motors(){
  pinMode(MOTOR_1_A,OUTPUT);
  pinMode(MOTOR_1_B,OUTPUT);
  analogWriteFrequency(MOTOR_1_A,100000);
  analogWriteFrequency(MOTOR_1_B,100000);

  pinMode(MOTOR_2_A,OUTPUT);
  pinMode(MOTOR_2_B,OUTPUT);
  analogWriteFrequency(MOTOR_2_A,100000);
  analogWriteFrequency(MOTOR_2_B,100000);

  pinMode(MOTOR_3_A,OUTPUT);
  pinMode(MOTOR_3_B,OUTPUT);
  analogWriteFrequency(MOTOR_3_A,100000);
  analogWriteFrequency(MOTOR_3_B,100000);

  pinMode(MOTOR_4_A,OUTPUT);
  pinMode(MOTOR_4_B,OUTPUT);
  analogWriteFrequency(MOTOR_4_A,100000);
  analogWriteFrequency(MOTOR_4_B,100000);

  analogWriteResolution(8);
}

void Motors::motorMove(int motor, int pot){
  if(pot > 0){
    analogWrite(motors[motor][0], pot);
    analogWrite(motors[motor][1], 0);
  }
  else {
    analogWrite(motors[motor][0], 0);
    analogWrite(motors[motor][1], abs(pot));
  }
}
void Motors::omni(int angle, int power,int rotation){
  if(angle == BALL_OUT_OF_RANGE){
    float output = constrain((rotation * 12), -165, 165) + (((rotation - prevRot) / 2) * 64);
    prevRot = rotation;

    constrain(output, -180, 180);

    motorMove(0, int(output)); //back right
    motorMove(1, int(output)); //front right
    motorMove(2, int(output)); //front left
    motorMove(3, int(output)); //back left
  }
  else{
    float A = power * cos((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);
    float B = -power * sin((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);
    float C = power * sin((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);
    float D = -power * cos((MOTOR_ANGLE_OFFSET - angle) * PI / 180.0);

    Serial.print(A); Serial.print('\t');
    Serial.print(B); Serial.print('\t');
    Serial.print(C); Serial.print('\t');
    Serial.print(D); Serial.print('\t');
    Serial.println("");

    float maxVal = max(max(abs(A), abs(B)), max(abs(C), abs(D))); //Serial.print(maxVal); Serial.print('\t');
    if (maxVal > 0) {
      float scale = float(power) / maxVal;
      //Serial.print(scale); Serial.print('\n');
      A = A * scale;
      B = B * scale;
      C = C * scale;
      D = D * scale;
    }

    motorMove(0, A + rotation); //back right
    motorMove(1, B + rotation); //front right
    motorMove(2, C + rotation); //front left
    motorMove(3, D + rotation); //back left
  }
}
