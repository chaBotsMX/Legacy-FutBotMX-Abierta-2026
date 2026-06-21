#include <Arduino.h>
#include "Omni.h"
#include "BohleBots_BNO055.h"
#include "Wire.h"
#include "camera/OpenMVStream.h"
#include "utils.h"

#define LINESERIAL Serial8

OpenMVStream front(Serial2);
OpenMVStream back(Serial1);


int ballAngle = 500;
int goalAngle = 500;

int lastBallX = 500;
int lastBallY = 500;
int lastGoalX = 500;
int lastGoalY = 500;
Omni Drive;
BNO bno;
int setPointGoalY = 203;
bool play = 0;
int setPoint = 0;

const uint8_t PLAY_BUTTON_PIN = 4;
const unsigned long DEBOUNCE_DELAY_MS = 50;
const int GOAL_CENTER_X = 160;
const int GOAL_CENTER_X_TOLERANCE = 4;
const int GOAL_CENTER_MIN_POWER = 80;
const int GOAL_CENTER_MAX_POWER = 200;
const int GOAL_MIN_X_LEFT = 70;
const int GOAL_MAX_X_RIGHT = 270;
const int FORWARD_ENTER_GOAL_Y_ERROR = 6;
const int FORWARD_EXIT_GOAL_Y_ERROR = 20;
const unsigned long BALL_MEMORY_MS = 3000;
bool playButtonState = LOW;
bool lastPlayButtonReading = LOW;
bool keepMovingForward = false;
unsigned long lastPlayButtonChange = 0;
unsigned long lastBallSeenAt = 0;

int calcOrientationError(){
  return wrap180(setPoint - bno.getHeadingAuto(100));
  
}
void setup() {
  front.begin(115200);
  back.begin(115200);
  Serial.begin(115200);
  Wire2.begin();  
  Wire2.setClock(400000);
  delay(2000);
  bno.startBNO(200,false);
  bno.saveOffsets(100);
  pinMode(13,OUTPUT);
  pinMode(PLAY_BUTTON_PIN, INPUT);
  pinMode(5, INPUT);

  lastPlayButtonReading = digitalRead(PLAY_BUTTON_PIN);
  playButtonState = lastPlayButtonReading;

}

void loop() {  
  digitalWrite(13,HIGH);
  if(back.update()){
    if(back.goalY != 500){
      lastGoalX = back.goalX;
      lastGoalY = back.goalY;
      float temp = atan2(lastGoalY,lastGoalX);
      goalAngle = int(degrees(temp));
      goalAngle = wrap360(goalAngle + 135);
      Serial.println(lastGoalX);
    }
  }
  if(front.update()){
    if(front.ballX != 500){
      lastBallX = front.ballX;
      lastBallY = front.ballY;
      lastBallSeenAt = millis();
      float temp = atan2(lastBallY,lastBallX);
      ballAngle = int(degrees(temp));
      ballAngle = wrap360(ballAngle - 45);
      //Serial.println(ballAngle);
    }
  }
  if(lastBallX != 500 && millis() - lastBallSeenAt > BALL_MEMORY_MS){
    lastBallX = 500;
    lastBallY = 500;
    ballAngle = 500;
  }
  if(digitalRead(5)){
    Serial.println("5");
    setPoint = bno.getHeadingAuto(100);
    lastGoalX = 500;
    lastGoalY = 500;
  }
  bool playButtonReading = digitalRead(PLAY_BUTTON_PIN);
  if(playButtonReading != lastPlayButtonReading){
    lastPlayButtonChange = millis();
  }

  if(millis() - lastPlayButtonChange >= DEBOUNCE_DELAY_MS){
    if(playButtonReading != playButtonState){
      playButtonState = playButtonReading;

      // Cambia el estado una sola vez al soltar el boton.
      if(playButtonState == LOW){
        play = !play;
      }
    }
  }
  lastPlayButtonReading = playButtonReading;
  int goalYError = 500;
  if(back.goalY != 500){
    goalYError = back.goalY - setPointGoalY;  
  }
  int goalCenterError = GOAL_CENTER_X - lastGoalX;
  bool ballCurrentlyVisible = front.ballX != 500;
  bool ballIsAvailable = lastBallX != 500;
  bool ballIsInForwardCone = ballAngle < 10 || ballAngle > 350;
  bool ballIsAhead = ballCurrentlyVisible && ballIsInForwardCone;

  if(!play || !ballIsAhead || goalYError == 500){
    keepMovingForward = false;
  }
  else if(keepMovingForward){
    if(goalYError >= FORWARD_EXIT_GOAL_Y_ERROR){
      keepMovingForward = false;
    }
  }
  else if(goalYError <= FORWARD_ENTER_GOAL_Y_ERROR){
    keepMovingForward = true;
  }
  //Serial.println(goalYError);

  if(play){
    if(goalYError == 500 && lastGoalY == 500){
      Drive.move(180,180,calcOrientationError());
    }
    else if(lastGoalX < GOAL_MIN_X_LEFT){
      Drive.move(270,GOAL_CENTER_MAX_POWER,calcOrientationError());
    }
    else if(lastGoalX > GOAL_MAX_X_RIGHT && lastGoalX != 500){
      Drive.move(90,GOAL_CENTER_MAX_POWER,calcOrientationError());
    }
    else if(keepMovingForward){
      Drive.move(0,220,calcOrientationError());
    }
    else if(goalYError > 6 && goalYError != 500){
      Drive.move(180,180,calcOrientationError());
    }
    else if(goalYError < -1){
      Drive.move(0,180,calcOrientationError());
    }
    else if(ballIsAvailable && back.goalY != 500 &&
            (ballCurrentlyVisible || !ballIsInForwardCone)){
      if(ballAngle >= 10 && ballAngle <= 180){
        Drive.move(90,190,calcOrientationError());
      }
      else if(ballAngle <= 350 && ballAngle > 180){
        Drive.move(270,190,calcOrientationError());
      }
      else{
        Drive.move(0,220,calcOrientationError());
      }
    }
    else if(lastGoalY != 500){
      if(abs(goalCenterError) <= GOAL_CENTER_X_TOLERANCE){
        Drive.move(500,0,calcOrientationError());
      }
      else{
        int centerPower = constrain(abs(goalCenterError) * 5,
                                    GOAL_CENTER_MIN_POWER,
                                    GOAL_CENTER_MAX_POWER);
        int centerDirection = goalCenterError > 0 ? 270 : 90;
        Drive.move(centerDirection,centerPower,calcOrientationError());
      }
    }
    else{
      Drive.move(500,0,calcOrientationError());
    }
  }
  else{
    Drive.move(500,0,calcOrientationError());
  }
}
