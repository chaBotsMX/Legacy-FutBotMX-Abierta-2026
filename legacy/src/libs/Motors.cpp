/**
 * @file Motors.cpp
 * @brief Methods to control the motors
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 29/03/26
 */

#include "Motors.h"

Motors::Motors(){
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);     
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  analogWriteFrequency(2,100000);
  analogWriteFrequency(3,100000);
  analogWriteFrequency(4,100000);
  analogWriteFrequency(5,100000);
  analogWriteFrequency(8,100000);
  analogWriteFrequency(9,100000);      
  analogWriteFrequency(10,100000);
  analogWriteFrequency(11,100000);
  analogWriteResolution(8);
}

void Motors::motorMove(int mot, int pot){
  if(pot > 0){
    analogWrite(motor[mot][0],pot);
    analogWrite(motor[mot][1],0);
  }
  else if(pot < 0){
    analogWrite(motor[mot][0],0);
    analogWrite(motor[mot][1],abs(pot));
  }
  else{
    analogWrite(motor[mot][0],0);
    analogWrite(motor[mot][1],0);
  }
}
void Motors::omni(int angle, int power,int rotation){
  if(angle == 500){
    float OutPut = constrain((rotation * 12),-165,165) + (((rotation -prevRot) / 2) * 64);
    prevRot = rotation;
    constrain(OutPut,-180,180);
    motorMove(0, 0 + int(OutPut)); //back right
    motorMove(1, 0 + int(OutPut)); //front right
    motorMove(2, 0 + int(OutPut)); //front left
    motorMove(3, 0 + int(OutPut)); //back left
  }
  else{
  float B = -power * sin((-angle + 45) * PI / 180.0);
  float D = -power * cos((-angle + 45) * PI / 180.0);
  float C = power * sin((-angle + 45) * PI / 180.0);
  float A = power * cos((-angle + 45) * PI / 180.0);

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