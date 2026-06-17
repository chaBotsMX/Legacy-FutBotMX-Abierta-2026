//La pcb superior que manda anuglo esta conectada al Serial3
//la pcb de linea al seral4

#include "MainInstance/MainInstance.h"

BNO bno;
elapsedMillis haveBallTimer;
elapsedMillis kickTestTimer = 0;
MainInstance::MainInstance() {}

void MainInstance::getReferenceBNO(Data &info){
  info.setPoint = bno.getHeadingAuto(100);
}

int MainInstance::getOrientationError(Data &info){
  return wrap180(info.heading - info.setPoint);
}

int MainInstance::correctLineAngle(int angle) {
  // Ajusta el ángulo recibido al marco de referencia del drive.
  if(angle == OUT_OF_RANGE) return OUT_OF_RANGE;
  return (angle + 90) % 360;
}

void MainInstance::filterData() {
    data.lineAng = correctLineAngle(data.lineAng);
    data.haveBall = vision.filterHaveBall(data.haveBall, haveBallTimer,data);
    data.frontGoal.coordinates.angle = vision.getGoalAngle(data.frontGoal.x, data.frontGoal.y);
    data.frontGoal.coordinates.magnitude = getEuclideanDistance(data.frontGoal.x, data.frontGoal.y);
    data.backGoal.coordinates.angle  = wrap360(vision.getGoalAngle(data.backGoal.x, data.backGoal.y)+180);
    data.backGoal.coordinates.magnitude = getEuclideanDistance(data.backGoal.x, data.backGoal.y);    
    data.frontGoal.proportionY = vision.getProportionY(data.frontGoal);
    data.frontGoal.proportionX = vision.getProportionX(data.frontGoal);
    data.backGoal.proportionY = vision.getProportionY(data.backGoal);
    data.backGoal.proportionX = vision. getProportionX(data.backGoal);
}

void MainInstance::init()
{  
  delay(2000);
  pinMode(13,OUTPUT);
  pinMode(1,INPUT);


  Wire.begin(); 
  Wire.setClock(400000); 
  delay(1000);
  Serial.begin(115200); 

  COMM.init();

  bno.startBNO(200, false); 
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100); 
  delay(1000);
  pinMode(0,INPUT);
  getReferenceBNO(data);
}

void MainInstance::update() 
{   
    digitalWrite(13,HIGH);
    data.heading = bno.getHeadingAuto(100);
    data.orientationError = getOrientationError(data);
    
    if(digitalRead(0)){
      getReferenceBNO(data);
      if(kickTestTimer > 3000){
        KD.sendKick();
      }
    }
    else{
      kickTestTimer = 0;
    }

    COMM.updateUARTData(data);
    filterData();

    Serial.print("Ball Angle: ");
    Serial.print(data.ballAng);
    Serial.print(" | Goal Angle: ");
    Serial.print(data.frontGoal.coordinates.angle);
    Serial.print(" | Have Ball: ");
    Serial.println(data.haveBall);
    Serial.print(" | goalX: ");
    Serial.print(data.frontGoal.x);
    Serial.print(" | goalY: ");
    Serial.println(data.frontGoal.y);
    Serial.print(" | Proportion Y: ");
    Serial.println(data.frontGoal.proportionY, 2);
    Serial.print(" | Proportion X: ");
    Serial.println(data.frontGoal.proportionX, 2);
    Serial.print( " magnitud: ");
    Serial.println(data.frontGoal.coordinates.magnitude);
    Serial.print( " lineAng: ");
    Serial.println(data.lineAng);
    Serial.print( " ballMag: ");
    Serial.println(data.ballDistance);
        
    Strategy strategy = Play.decideStrategy(data);

    Serial.print(" | strategy: ");
    Serial.print(Play.getStrategyName(strategy));
    

  if(digitalRead(1)){
  //Serial.println(error);
  Play.executeStrategy(strategy);
  }
  else{
    drive.move(0, 0, 0);
  }

    Serial.print(" | scoringState: ");
    Serial.println(Play.getScoringStateName());
}


