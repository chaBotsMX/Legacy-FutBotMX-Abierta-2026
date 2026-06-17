#include "visionProccesing.h"

VisionProccesing::VisionProccesing() {}

float VisionProccesing::getProportionY(GoalInfo &goal) {
    if (goal.x == 500 || goal.y == 500 || goal.x == 0) return 500;
    return float(goal.y) / float(goal.x);
}

float VisionProccesing::getProportionX(GoalInfo &goal) {
    if (goal.x == 500 || goal.y == 500 || goal.y == 0) return 500;
    return float(goal.x) / float(goal.y);
}


bool VisionProccesing::isInLine(Data &verify) {
    return verify.lineAng != 500;
}

bool VisionProccesing::isSeeingBall(Data &verify) {
    return verify.ballAng != 500;
}

bool VisionProccesing::isHaveBall(Data &verify) {
    return verify.haveBall;
}

bool VisionProccesing::isSeeingBothGoals(Data &verify) {
    return verify.frontGoal.coordinates.angle != 500 && verify.backGoal.coordinates.angle != 500;
}
bool VisionProccesing::isSeeingBackGoal(Data &verify) {
    return verify.backGoal.coordinates.angle != 500;
}
bool VisionProccesing::isSeeingFrontGoal(Data &verify) {
    return verify.frontGoal.coordinates.angle != 500;
}

int VisionProccesing::getBallCatchTrajectory(int ballAngle, int ballDistance) {
  if (ballAngle == 500) return 500;
  if (ballDistance == 500) return ballAngle;

  float closeFactor = constrain((400.0f - ballDistance) / 400.0f, 0.0f, 1.0f);
  int offset = 90 * closeFactor;

  if (ballAngle > 20 && ballAngle < 180) {
    return wrap360(ballAngle + offset);
  }
  else if (ballAngle >= 180 && ballAngle < 340) {
    return wrap360(ballAngle - offset);
  }

  return 0;
}

int VisionProccesing::getBallCatchSpeed(int ballAngle, int ballDistance) {
  if (ballDistance == OUT_OF_RANGE) return BASE_BALL_IN_FRONT_PWM;

  float normalizedDistance = constrain(float(ballDistance) / 400.0f, 0.0f, 1.0f);
  float nonLinearResponse = pow(normalizedDistance, 0.45f);

  if (ballAngle < 330 && ballAngle > 30){
    return constrain(
      BASE_CHASE_PWM + int((230 - BASE_CHASE_PWM) * nonLinearResponse),
      0,
      230
    );
  }

  return constrain(
    BASE_BALL_IN_FRONT_PWM + int((230 - BASE_BALL_IN_FRONT_PWM) * nonLinearResponse),
    0,
    230
  );
}

int VisionProccesing::getErrorTowardsGoal(GoalInfo &goal, int currentHeading) {
  if (goal.coordinates.angle == 500) return 0; // No hay información válida
  int error = wrap180(goal.coordinates.angle - currentHeading); // Suponiendo que el drive considera 0° como "hacia adelante"
  return error;
}


bool VisionProccesing::filterHaveBall(bool haveBall, elapsedMillis &timer, Data &verify) {
  if (verify.ballAng == 500 || (verify.ballAng > 30 && verify.ballAng < 320)) {
    return false; // Si no vemos la pelota, asumimos que no la tenemos
  }
  if (haveBall) {
    timer = 0; 
    return true;
  } else {
    return timer < BALL_DEBOUNCE_MS; 
  }
}


int VisionProccesing::getGoalAngle(int inputX, int inputY) {
    // Convierte las coordenadas del objetivo en un ángulo relativo al drive.
    if (inputX == 500 || inputY == 500) {
        return 500; // No hay información válida
    }
    int resultAng = atan2(inputY - 120 ,inputX - 160) * 180 / PI;
    return wrap360(resultAng - 90);

}