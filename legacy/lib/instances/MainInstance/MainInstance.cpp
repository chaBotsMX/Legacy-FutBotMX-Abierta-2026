//La pcb superior que manda anuglo esta conectada al Serial4
//la pcb de linea al seral3

#include "MainInstance/MainInstance.h"

BNO bno;
Omni drive;
elapsedMillis haveBallTimer;

MainInstance::MainInstance() {}

void MainInstance::getReferenceBNO(Data &info){
  info.setPoint = bno.getHeadingAuto(100);
}

int MainInstance::getOrientationError(Data &info){
  return wrap180(info.setPoint - info.heading);
}

int MainInstance::decideStrategy() {
    if (vision.isInLine(data)) {
        return 1; // Salir de linea
    } else if (vision.isHaveBall(data)) {
        return 2; // meter gol
    } else if (vision.isSeeingBall(data)) {
        return 3; // ir hacia la pelota
    }
    else if(vision.isSeeingBothGoals(data)){
        return 4;  // reubicarse usando ambas porterias como referencia 
    }
    else if(vision.isSeeingBackGoal(data)){
        return 5; // reubicarse usando porteria trasera como referencia
    }
    else if(vision.isSeeingFrontGoal(data)){
        return 6; // reubicarse usando porteria frontal como referencia
    }
    else{
        return 0;  // quieto a la espera de información
    }
}

void MainInstance::executeStrategy(int strategy) {
    switch (strategy) {
        case 1:
            drive.move(data.lineAng, 200, data.orientationError); // Lógica para salir de línea
            Serial4.write(3);
            break;
        case 2:
            tryScoring(data);// Lógica para meter gol
            break;
        case 3:
            Serial4.write(3);
            drive.move(vision.getBallCatchTrajectory(data.ballAng, data.ballDistance), vision.getBallCatchSpeed(data.ballAng, data.ballDistance), data.orientationError); // Lógica para ir hacia la pelota
            break;
        case 4:
        case 5:
        case 6:
            Serial4.write(3);
            // Lógica para estar quieto a la espera de información
            drive.move(180, 160, data.orientationError);
            break;
        default:
            Serial4.write(3);
            // Lógica para estar quieto a la espera de información
            drive.move(180, 160, data.orientationError);
            break;
    }
}

bool MainInstance::receiveInfoFromCameraTeensy(HardwareSerial &port, Data &info) {
  static uint8_t state = 0;
  static Data incoming;

  while (port.available()) {
    uint8_t byteIn = port.read();

    switch (state) {
      case 0: // Esperando START_BYTE
        if (byteIn == START_BYTE) {
          incoming = Data(); // limpia datos temporales
          state = 1;
        }
        break;

      case 1: // ballAng / 2
        incoming.ballAng = byteIn * 2;
        state = 2;
        break;

      case 2: // haveBall
        incoming.haveBall = byteIn != 0;
        state = 3;
        break;

      case 3: // ballDistance / 2
        incoming.ballDistance = byteIn * 2;
        state = 4;
        break;

      case 4: // front.goalX / 2
        incoming.frontGoal.x = byteIn * 2;
        state = 5;
        break;

      case 5: // front.goalY / 2
        incoming.frontGoal.y = byteIn * 2;
        state = 6;
        break;

      case 6: // back.goalX / 2
        incoming.backGoal.x = byteIn * 2;
        state = 7;
        break;

      case 7: // back.goalY / 2
        incoming.backGoal.y = byteIn * 2;
        state = 8;
        break;

      case 8: // Esperando END_BYTE
        if (byteIn == END_BYTE) {
          info.ballAng = incoming.ballAng;
          info.haveBall = incoming.haveBall;
          info.ballDistance = incoming.ballDistance;

          info.frontGoal.x = incoming.frontGoal.x;
          info.frontGoal.y = incoming.frontGoal.y;

          info.backGoal.x = incoming.backGoal.x;
          info.backGoal.y = incoming.backGoal.y;

          state = 0;
          return true;
        } else {
          state = 0;

          // Si este byte corrupto era realmente otro inicio de paquete,
          // no lo perdemos.
          if (byteIn == START_BYTE) {
            incoming = Data();
            state = 1;
          }
        }
        break;

      default:
        state = 0;
        break;
    }
  }

  return false;
}
int MainInstance::correctLineAngle(int angle) {
  // Ajusta el ángulo recibido al marco de referencia del drive.
  if(angle == 500) return 500;
  return (angle + 90) % 360;
}



void MainInstance::turnTowardsGoalSmooth(GoalInfo &goal) {
  if (goal.coordinates.angle == 500) {
    smoothGoalAngle(500, 0);
    return;
  }

  int smoothedGoalError = smoothGoalAngle(goal.coordinates.angle, 0);

  if (smoothedGoalError == 500) return;

  int rotation = -smoothedGoalError;

  drive.move(500, 0, rotation);
}



int MainInstance::smoothGoalAngle(int setPointDif, int currentSetPoint, int heading) {
  static int objective = 0;
  static int actualObjective = 0;
  static bool initialized = false;
  static elapsedMillis changeTimer;

  if (setPointDif == 500 || currentSetPoint == 500 || heading == 500) {
    objective = 0;
    actualObjective = 0;
    initialized = false;
    changeTimer = 0;
    return 500;
  }


  objective = wrap360(currentSetPoint - setPointDif);

  if (!initialized) {
    actualObjective = objective;
    initialized = true;
    changeTimer = 0;
  }

  if (changeTimer >= 100) {
    int difference = wrap180(objective - actualObjective);

    if (difference > 0) {
      actualObjective++;
    } else if (difference < 0) {
      actualObjective--;
    }

    actualObjective = wrap360(actualObjective);
    changeTimer = 0;
  }

  return wrap180(actualObjective - heading);
}
void MainInstance::goToGoal(Data &info) {
  if (info.frontGoal.coordinates.angle == 500) return; // No hay información válida

  int goalError = vision.getErrorTowardsGoal(info.frontGoal, data.orientationError);

  drive.move(info.frontGoal.coordinates.angle,180, goalError);
}
void MainInstance::tryScoring(Data &info) {
  Serial4.write(2);

  if (info.frontGoal.coordinates.angle == 500 || info.frontGoal.proportionX < 0.3) {
    drive.move(180, 150, info.orientationError); 
  }
  else if (info.frontGoal.proportionX >= 0.3) {
    turnTowardsGoalSmooth(info.frontGoal);
    int kickError = abs(wrap180(info.frontGoal.coordinates.angle));
    if (kickError < 10) {
      Serial4.write(1);
    }
  }

}

void MainInstance::filterData() {
    data.lineAng = correctLineAngle(data.lineAng);
    data.haveBall = vision.filterHaveBall(data.haveBall, haveBallTimer,data);
    data.frontGoal.coordinates.angle = vision.getGoalAngle(data.frontGoal.x, data.frontGoal.y);
    data.frontGoal.coordinates.magnitude = getEuclideanDistance(data.frontGoal.x, data.frontGoal.y);
    data.backGoal.coordinates.angle  = vision.getGoalAngle(data.backGoal.x, data.backGoal.y);
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
  Serial.begin(115200); 
  Serial4.begin(2000000);
  Serial3.begin(2000000);
  bno.startBNO(200, false); 
  Serial.println("Fully Calibrated!");  
  bno.saveOffsets(100); 
  delay(1000);
  pinMode(0,INPUT);
 
}

bool MainInstance::receiveInfoFromLineTeensy(HardwareSerial &port) {
  while(port.available()){
    int rawAngle = port.read() * 2;
    if(rawAngle != 500){
      //lineAng = correctLineAngle(rawAngle);
      data.lineAng = 500;
    }
    else{
      data.lineAng = 500;
    }
    digitalWrite(13,HIGH);
    Serial.println(data.lineAng);
    return true;
  }
  return false;
}

void MainInstance::getIncomingData() {
    receiveInfoFromLineTeensy(Serial3);
    receiveInfoFromCameraTeensy(Serial4, data);
}



void MainInstance::update() 
{   
    digitalWrite(13,HIGH);
    data.heading = bno.getHeadingAuto(100);
    data.orientationError = getOrientationError(data);
    //Leer boton 
    if(digitalRead(0)){
      getReferenceBNO(data);
    }

    getIncomingData();
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
    if(digitalRead(1)){
    //Serial.println(error);
    executeStrategy(decideStrategy());
    }
    else{
      drive.move(0, 0, 0);
    }
}
