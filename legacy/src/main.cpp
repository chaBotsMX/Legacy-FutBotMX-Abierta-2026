//La pcb superior que manda anuglo esta conectada al Serial3
//la pcb de linea al seral4
#include <BohleBots_BNO055.h>
#include <Wire.h>
#include "Omni.h"
BNO bno;
Omni robot;


int ballAng = 500;
int goalAng = 500;
int lineAng = 500;
bool haveBall = false;


#define START_BYTE 0xAA
#define END_BYTE   0x55

int receivedBallAngle = 500;
bool receivedHaveBall = false;
int receivedGoalAngle = 500;


int smoothGoalAng = 500;
unsigned long lastGoalStep = 0;
const int GOAL_STEP_MS = 25 ; 

int wrap180(int angle) {
  while (angle > 180) angle -= 360;
  while (angle < -180) angle += 360;
  return angle;
}

int wrap360(int angle) {
  while (angle < 0) angle += 360;
  while (angle >= 360) angle -= 360;
  return angle;
}

bool receiveInfoFromCameraTeensy(HardwareSerial &port) {
  static uint8_t state = 0;
  static uint8_t ballByte = 0;
  static uint8_t haveBallByte = 0;
  static uint8_t goalByte = 0;

  while (port.available()) {
    uint8_t data = port.read();

    switch (state) {
      case 0: // Esperando 0xAA
        if (data == START_BYTE) {
          state = 1;
        }
        break;

      case 1: // Recibir ballAng / 2
        ballByte = data;
        state = 2;
        break;

      case 2: // Recibir haveBall()
        haveBallByte = data;
        state = 3;
        break;

      case 3: // Recibir getGoalAngle() / 2
        goalByte = data;
        state = 4;
        break;

      case 4: // Esperar 0x55
        if (data == END_BYTE) {
          receivedBallAngle = ballByte * 2;
          receivedHaveBall = haveBallByte != 0;
          receivedGoalAngle = goalByte * 2;

          state = 0;
          return true; // Paquete válido recibido
        } else {
          // Paquete corrupto, reiniciar
          state = 0;

          // Si justo este byte era otro start byte, no lo pierdas
          if (data == START_BYTE) {
            state = 1;
          }
        }
        break;
    }
  }

  return false; // Todavía no hay paquete completo
}
static int correctLineAngle(int angle) {
  // Ajusta el ángulo recibido al marco de referencia del robot.
  return (angle + 90) % 360;
}

void goToGoal() {
  if (goalAng != 500 && goalAng > 30  && goalAng < 330) {
    int heading = wrap360(bno.getRLHeading());

    if (smoothGoalAng == 500) {
      smoothGoalAng = heading;
    }

    if (millis() - lastGoalStep >= GOAL_STEP_MS) {
      lastGoalStep = millis();

      int diff = wrap180(goalAng - smoothGoalAng);

      if (diff > 0) {
        smoothGoalAng = wrap360(smoothGoalAng + 1);
      } else if (diff < 0) {
        smoothGoalAng = wrap360(smoothGoalAng - 1);
      }
    }

    int goalError = wrap180(heading - smoothGoalAng);
    int realGoalError = wrap180(heading - goalAng);

    robot.move(0, 160, goalError);

    if (abs(realGoalError) < 10) {
      Serial4.write(1);
    }

  } else {
    int corr = bno.getRLHeadingAuto(100);
    robot.move(180, 150, corr);
  }
}
void setup()
{  
  delay(2000);
  pinMode(13,OUTPUT);
  pinMode(1,INPUT);

  Wire.begin(); 
  Wire.setClock(400000); 
  Serial.begin(115200); 
  Serial4.begin(115200);
  Serial3.begin(2000000);
  bno.startBNO(200, false); 
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100); 
  delay(1000);
  pinMode(0,INPUT);
 
}

void loop() 
{
  
  int error = bno.getRLHeadingAuto(100);

  if(digitalRead(0)){
    bno.setReference();
  }

  while(Serial3.available()){
    int rawAngle = Serial3.read() * 2;
    if(rawAngle != 500){
      //lineAng = correctLineAngle(rawAngle);
      lineAng = 500;
    }
    else{
      lineAng = 500;
    }
    digitalWrite(13,HIGH);
    Serial.println(lineAng);
  }
  //Serial.println(error, DEC); 
  //Serial.println(digitalRead(1)); 
  receiveInfoFromCameraTeensy(Serial4);
  ballAng = receivedBallAngle;
  receivedGoalAngle == 306 ? goalAng = 500 : goalAng = receivedGoalAngle;
  haveBall = receivedHaveBall;

  Serial.print("Ball Angle: ");
  Serial.print(ballAng);
  Serial.print(" | Goal Angle: ");
  Serial.print(goalAng);
  Serial.print(" | Have Ball: ");
  Serial.println(haveBall);
  if(digitalRead(1)){
  //Serial.println(error);
    if(lineAng != 500){
      robot.move(lineAng,200,error);
    }
    else{
     if(ballAng != 500) {
      if(ballAng > 330 || ballAng < 30 && !haveBall){ 
        robot.move(ballAng,170,error);
        Serial4.write(3);
      }
      else if(!haveBall){
        robot.move(ballAng,200,error);
        Serial4.write(3);
      }
      else{
        Serial4.write(2);
        goToGoal();
      }
    }
    else{
      Serial4.write(3);
      robot.move(180,200,error);
    }

  
    }
  }
  else{
  robot.move(0,0,0);
  }
}
