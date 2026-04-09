//La pcb superior que manda anuglo esta conectada al Serial3
//la pcb de linea al seral4
#include <BohleBots_BNO055.h>
#include <Wire.h>
#include <Motors.h>

#define STARTUP_DELAY 2000
#define CALIBRATION_DELAY 1000

#define LED_PIN 13
#define MOTOR_BUTTON_PIN 1
#define ORIENTATION_BUTTON_PIN 0

#define BALL_OUT_OF_RANGE 500

BNO bno;
Motors robot;

int ballAng = BALL_OUT_OF_RANGE;
int lineAng = BALL_OUT_OF_RANGE;

void setup()
{  
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_BUTTON_PIN, INPUT);
  pinMode(ORIENTATION_BUTTON_PIN, INPUT);

  delay(STARTUP_DELAY);

  Wire.begin(); 
  Wire.setClock(400000); 

  Serial.begin(115200); 
  Serial4.begin(2000000);
  Serial3.begin(115200);

  bno.startBNO(200, false); 
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100); 

  delay(CALIBRATION_DELAY);
}

void loop() 
{
  int error = bno.getRLHeadingAuto(100);
  while(Serial3.available()){
    int a = Serial3.read();
    ballAng = a * 2;
    Serial.println(ballAng);
  }
  if(digitalRead(0)){
    bno.setReference();
  }

  while(Serial4.available()){
    lineAng = Serial4.read() * 2;
    digitalWrite(LED_PIN,HIGH);
  }
  //Serial.println(error, DEC); 
  //Serial.println(digitalRead(1)); 
  if(digitalRead(1)){
  //Serial.println(error);
    if(lineAng != 500){
      robot.omni(lineAng, 200, error);
    }
    else{
      ballAng != BALL_OUT_OF_RANGE ? robot.omni(ballAng, 200, error): robot.omni(180, 200, error );
    }
  }
  else robot.omni(0, 0, 0);
}

