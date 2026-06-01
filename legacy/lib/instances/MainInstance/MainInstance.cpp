//La pcb superior que manda anuglo esta conectada al Serial4
//la pcb de linea al seral3

#include "MainInstance/MainInstance.h"

BNO bno;
Omni drive;
Data data;
elapsedMillis haveBallTimer;

MainInstance::MainInstance() {}

bool MainInstance::isInLine() {
    return data.lineAng != 500;
}

bool MainInstance::isSeeingBall() {
    return data.ballAng != 500;
}

bool MainInstance::isHaveBall() {
    return data.haveBall;
}

bool MainInstance::isSeeingBothGoals() {
    return data.frontGoal.coordinates.angle != 500 && data.backGoal.coordinates.angle != 500;
}
int MainInstance::decideStrategy() {
    if (isInLine()) {
        return 1; // Salir de linea
    } else if (isHaveBall()) {
        return 2; // meter gol
    } else if (isSeeingBall()) {
        return 3; // ir hacia la pelota
    }
    else if(isSeeingBothGoals()){
        return 4;  // reubicarse usando ambas porterias como referencia 
    }
    else if(isSeeingBackGoal()){
        return 5; // reubicarse usando porteria trasera como referencia
    }
    else if(isSeeingFrontGoal()){
        return 6; // reubicarse usando porteria frontal como referencia
    }
    else{
        return 0;  // quieto a la espera de información
    }

  
      
}

int MainInstance::getBallCatchTrajectory(int ballAngle, int ballDistance) {
  if (ballAngle == 500) return 500;
  if (ballDistance == 500) return ballAngle;

  float closeFactor = constrain((320.0f - ballDistance) / 320.0f, 0.0f, 1.0f);
  int offset = 90 * closeFactor;

  if (ballAngle > 20 && ballAngle < 180) {
    return wrap360(ballAngle + offset);
  }
  else if (ballAngle >= 180 && ballAngle < 340) {
    return wrap360(ballAngle - offset);
  }

  return 0;
}
int MainInstance::getBallCatchSpeed(int ballAngle, int ballDistance) {
  if (ballDistance == 500) return 160; 
  if (ballAngle > 330 || ballAngle < 30){
    return 150 + int(0.05 * float(ballDistance));
  }
  return 200;
}
void MainInstance::executeStrategy(int strategy) {
    switch (strategy) {
        case 1:
            drive.move(data.lineAng, 200, data.orientationError); // Lógica para salir de línea
            Serial4.write(3);
            break;
        case 2:
            goToGoal(data);// Lógica para meter gol
            break;
        case 3:
            Serial4.write(3);
            drive.move(getBallCatchTrajectory(data.ballAng, data.ballDistance), getBallCatchSpeed(data.ballAng, data.ballDistance), data.orientationError); // Lógica para ir hacia la pelota
            break;
        case 4:
            // Lógica para reubicarse usando ambas porterías como referencia
            break;
        case 5:
            // Lógica para reubicarse usando portería trasera como referencia
            break;
        case 6:
            // Lógica para reubicarse usando portería frontal como referencia
            break;
        default:
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

int MainInstance::getEuclideanDistance(int x, int y) {
  if (x == 500 || y == 500) {
    return 500; // No hay información válida
  }
  return sqrt(pow(x - 160, 2) + pow(y - 120, 2));
}


Vector MainInstance::sumVectors(Vector &v1, Vector &v2) {
  if (v1.angle == 500 || v1.magnitude == 500 || v2.angle == 500 || v2.magnitude == 500) {
    return {500,500}; // No hay información válida
  }
  int x = v1.magnitude * cos(radians(v1.angle)) + v2.magnitude * cos(radians(v2.angle));
  int y = v1.magnitude * sin(radians(v1.angle)) + v2.magnitude * sin(radians(v2.angle));
  int resultAngle = atan2(y, x) * 180 / PI;
  return   {wrap360(resultAngle), (int)sqrt(x * x + y * y)};
}


void MainInstance::goToGoal(Data &info) {
  int goalAng = info.frontGoal.coordinates.angle;
  Serial4.write(2);
  if (goalAng != 500 && goalAng > 30 && goalAng < 330) {

    int heading = wrap360(bno.getRLHeading());

    if (info.smoothGoalAng == 500) {
      info.smoothGoalAng = heading;
    }

    if (millis() - info.lastGoalStep >= GOAL_STEP_MS) {
      info.lastGoalStep = millis();

      int diff = wrap180(goalAng - info.smoothGoalAng);

      if (diff > 0) {
        info.smoothGoalAng = wrap360(info.smoothGoalAng + 1);
      } else if (diff < 0) {
        info.smoothGoalAng = wrap360(info.smoothGoalAng - 1);
      }
    }

    int goalError = wrap180(heading - info.smoothGoalAng);
    int realGoalError = wrap180(heading - goalAng);

    drive.move(0, 160, goalError);

    if (abs(realGoalError) < 10) {
      Serial4.write(1);
    }

  } else {
    int corr = bno.getRLHeadingAuto(100);
    drive.move(180, 150, corr);

    info.smoothGoalAng = 500;
  }
}
bool MainInstance::filterHaveBall(bool haveBall, elapsedMillis &timer) {

  if (haveBall) {
    timer = 0; 
    return true;
  } else {
    return timer < BALL_DEBOUNCE_MS; 
  }
}
void MainInstance::filterData() {
    data.lineAng = correctLineAngle(data.lineAng);
    data.haveBall = filterHaveBall(data.haveBall, haveBallTimer);
    data.frontGoal.coordinates.angle = getGoalAngle(data.frontGoal.x, data.frontGoal.y);
    data.frontGoal.coordinates.magnitude = getEuclideanDistance(data.frontGoal.x, data.frontGoal.y);
    data.backGoal.coordinates.angle  = getGoalAngle(data.backGoal.x, data.backGoal.y);
    data.backGoal.coordinates.magnitude = getEuclideanDistance(data.backGoal.x, data.backGoal.y);    
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

int MainInstance::getGoalAngle(int inputX, int inputY) {
    // Convierte las coordenadas del objetivo en un ángulo relativo al drive.
    if (inputX == 500 || inputY == 500) {
        return 500; // No hay información válida
    }
    int resultAng = atan2(inputY - 120 ,inputX - 160) * 180 / PI;
    return wrap360(resultAng - 90);

}

void MainInstance::update() 
{  
    int error = bno.getRLHeadingAuto(100);
    data.orientationError = error;
    //Leer boton 
    if(digitalRead(0)){
    bno.setReference();
    }

    getIncomingData();
    filterData();

    Serial.print("Ball Angle: ");
    Serial.print(data.ballAng);
    Serial.print(" | Goal Angle: ");
    Serial.print(data.frontGoal.coordinates.angle);
    Serial.print(" | Have Ball: ");
    Serial.println(data.haveBall);
    if(digitalRead(1)){
    //Serial.println(error);
    executeStrategy(decideStrategy());
    }
    else{
      drive.move(0, 0, 0);
    }
}
