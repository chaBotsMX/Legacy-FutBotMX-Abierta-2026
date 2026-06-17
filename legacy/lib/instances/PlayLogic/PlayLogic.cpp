#include "PlayLogic.h"

Strategy PlayLogic::decideStrategy(Data &newData) {
  local = newData;

  if (vision.isInLine(local)) {
    return Strategy::EscapeLine;
  }
  else if (vision.isHaveBall(local)) {
    return Strategy::Score;
  }
  else if (vision.isSeeingBall(local)) {
    return Strategy::ChaseBall;
  }
  else if (vision.isSeeingBothGoals(local)) {
    return Strategy::RepositionWithBothGoals;
  }
  else if (vision.isSeeingBackGoal(local)) {
    return Strategy::RepositionWithBackGoal;
  }
  else if (vision.isSeeingFrontGoal(local)) {
    return Strategy::RepositionWithFrontGoal;
  }

  return Strategy::StandBy;
}

const char* PlayLogic::getStrategyName(Strategy strategy) const {
  switch (strategy) {
    case Strategy::StandBy:
      return "StandBy";
    case Strategy::EscapeLine:
      return "EscapeLine";
    case Strategy::Score:
      return "Score";
    case Strategy::ChaseBall:
      return "ChaseBall";
    case Strategy::RepositionWithBothGoals:
      return "RepositionWithBothGoals";
    case Strategy::RepositionWithBackGoal:
      return "RepositionWithBackGoal";
    case Strategy::RepositionWithFrontGoal:
      return "RepositionWithFrontGoal";
  }

  return "Unknown";
}

const char* PlayLogic::getScoringStateName() const {
  switch (scoringState) {
    case ScoringState::Idle:
      return "Idle";
    case ScoringState::ChooseMethod:
      return "ChooseMethod";
    case ScoringState::TakeOutOfCorner:
      return "TakeOutOfCorner";
    case ScoringState::CenterBeforeKick:
      return "CenterBeforeKick";
    case ScoringState::GoToGoal:
      return "GoToGoal";
    case ScoringState::AimAndKick:
      return "AimAndKick";
  }

  return "Unknown";
}

void PlayLogic::resetScoring() {
  scoringState = ScoringState::Idle;
  centerBeforeKickTimer = 0;
  smoothGoalAngle(OUT_OF_RANGE, OUT_OF_RANGE);
}

bool PlayLogic::shouldTakeOutOfCorner() const {
  return local.frontGoal.coordinates.angle == OUT_OF_RANGE ||
         local.frontGoal.proportionX == OUT_OF_RANGE ||
         local.frontGoal.proportionX < 0.3;
}

void PlayLogic::goToNeutralBack(){
  Vector neutralDirectionVector = sumVectors(neutralSoloBack, local.backGoal.coordinates);
  Serial.print("neutral angle : ");
  Serial.print(neutralDirectionVector.angle);
  Serial.print("  neutral magnitude: ");
  Serial.println(neutralDirectionVector.magnitude);
  if (neutralDirectionVector.magnitude > 7) {
    drive.move(neutralDirectionVector.angle, 200, local.orientationError);
  }
  else {
    drive.move(STAY_STILL, 0, local.orientationError);
  }

}

void PlayLogic::executeStrategy(Strategy strategy) {
  if (!vision.isHaveBall(local) || strategy == Strategy::EscapeLine) {
    resetScoring();
  }

  switch (strategy) {
    case Strategy::EscapeLine:
      executeEscapeLine();
      break;

    case Strategy::Score:
      updateScoring();
      break;

    case Strategy::ChaseBall:
      executeChaseBall();
      break;

    case Strategy::RepositionWithBothGoals:
      KD.turnDribblerOff();
      goToNeutralCenter ();
      break;
    case Strategy::RepositionWithBackGoal:
      KD.turnDribblerOff();
      goToNeutralBack ();
      break;
    case Strategy::RepositionWithFrontGoal:
      executeReposition();
      break;

    case Strategy::StandBy:
      executeStandBy();
      break;
  }
}

void PlayLogic::executeEscapeLine() {
  Serial.println(local.lineAng);
  drive.move(wrap360(local.lineAng +180), 200, local.orientationError);
  KD.turnDribblerOff();
}

void PlayLogic::executeChaseBall() {
  KD.turnDribblerOff();
  int goalOrientation = local.orientationError;

  if (local.frontGoal.coordinates.angle != OUT_OF_RANGE &&
      local.frontGoal.coordinates.magnitude > ROTATE_MAGNITUDE_TRESHOLD) {
    goalOrientation = wrap180(local.orientationError - constrain(wrap180(local.frontGoal.coordinates.angle), -MAX_ROTATION_TOWARDS_GOAL, MAX_ROTATION_TOWARDS_GOAL));
  }

  drive.move(
    vision.getBallCatchTrajectory(local.ballAng, local.ballDistance),
    vision.getBallCatchSpeed(local.ballAng, local.ballDistance),
    goalOrientation
  );
}

void PlayLogic::executeReposition() {
  KD.turnDribblerOff();
  goToNeutralFront();
}

void PlayLogic::executeStandBy() {
  KD.turnDribblerOff();   
  drive.move(180, 160, local.orientationError);
}

void PlayLogic::goToGoal() {
  int rotation = smoothGoalAngle(local.frontGoal.coordinates.angle,local.heading);
  if (local.frontGoal.coordinates.angle != OUT_OF_RANGE) {
    drive.move(0, GO_TO_GOAL_PWM, -rotation);
    int kickError = abs(wrap180(local.frontGoal.coordinates.angle));
    if (kickError < GO_TO_GOAL_KICK_ERROR) {
      KD.sendKick();
    }
  }
  else {
    drive.move(0, GO_TO_GOAL_PWM, local.orientationError);
  }
}

void PlayLogic::rotateAndKick() {
  int rotation = smoothGoalAngle(local.frontGoal.coordinates.angle,local.heading);

  if (rotation != OUT_OF_RANGE) {
    drive.move(STAY_STILL, 0, -rotation);
  }

  Serial.print("rotation: ");

  int kickError = abs(wrap180(local.frontGoal.coordinates.angle));
  Serial.println(rotation);
  if (kickError < ROTATE_KICK_ERROR) {
    KD.sendKick();
  }
}

void PlayLogic::goToNeutralFront() {
  Vector neutralDirectionVector = sumVectors(neutralSoloFront, local.frontGoal.coordinates);
  Serial.print("neutral angle : ");
  Serial.print(neutralDirectionVector.angle);
  Serial.print("  neutral magnitude: ");
  Serial.println(neutralDirectionVector.magnitude);
  if (neutralDirectionVector.magnitude > 7) {
    drive.move(neutralDirectionVector.angle, 200, local.orientationError);
  }
  else {
    drive.move(STAY_STILL, 0, local.orientationError);
  }
}


void PlayLogic::goToNeutralCenter(){
  Vector centerVector = sumVectors(local.frontGoal.coordinates,local.backGoal.coordinates);
  Vector neutralDirectionVector = sumVectors(neutralCenter, centerVector);
  Serial.print("front goal magnitude: ");
  Serial.print(local.frontGoal.coordinates.magnitude);
  Serial.print(" front goal angle: ");
  Serial.println(local.frontGoal.coordinates.angle);
  Serial.print("back goal magnitude: ");
  Serial.print(local.backGoal.coordinates.magnitude);
  Serial.print(" back goal angle: ");
  Serial.println(local.backGoal.coordinates.angle);
  Serial.print("neutral angle : ");
  Serial.print(neutralDirectionVector.angle);
  Serial.print("  neutral magnitude: ");
  Serial.println(neutralDirectionVector.magnitude);
  if (neutralDirectionVector.magnitude > 10) {
    drive.move(neutralDirectionVector.angle, 200, local.orientationError);
  }
  else {
    drive.move(STAY_STILL, 0, local.orientationError);
  }  
}
int PlayLogic::smoothGoalAngle(int goalAngle, int heading) {
  static float actualObjective = OUT_OF_RANGE;
  static elapsedMillis changeTimer = 0;
  static elapsedMillis lastCall = 0;

  if (goalAngle == OUT_OF_RANGE) {
    actualObjective = OUT_OF_RANGE;
    changeTimer = 0;
    lastCall = 0;
    return OUT_OF_RANGE;
  }

  if (lastCall > 100 || actualObjective == OUT_OF_RANGE) {
    actualObjective = heading;
    changeTimer = 0;
  }

  lastCall = 0;

  if (changeTimer > 500) {
    int error = wrap180(goalAngle);
    actualObjective = wrap360(actualObjective + constrain(error, -2, 2));
    changeTimer = 0;
  }

  return wrap180(actualObjective - heading);
}

void PlayLogic::updateScoring() {
  KD.turnDribblerOn();

  if (scoringState == ScoringState::Idle) {
    scoringState = ScoringState::ChooseMethod;
  }

  if (scoringState == ScoringState::ChooseMethod) {
    if (shouldTakeOutOfCorner()) {
      scoringState = ScoringState::TakeOutOfCorner;
    }
    else {
      scoringState = ScoringState::GoToGoal;
    }
  }

  switch (scoringState) {
    case ScoringState::Idle:
    case ScoringState::ChooseMethod:
      break;

    case ScoringState::TakeOutOfCorner:
      if (shouldTakeOutOfCorner()) {
        takeOutOfCorner();
      }
      else {
        centerBeforeKickTimer = 0;
        scoringState = ScoringState::CenterBeforeKick;
        centerBeforeKick();
      }
      break;

    case ScoringState::CenterBeforeKick:
      if (shouldTakeOutOfCorner()) {
        scoringState = ScoringState::TakeOutOfCorner;
        takeOutOfCorner();
      }
      else {
        centerBeforeKick();
      }
      break;

    case ScoringState::GoToGoal:
      goToGoal();
      break;

    case ScoringState::AimAndKick:
      rotateAndKick();
      break;
  }
}

void PlayLogic::takeOutOfCorner() {
  drive.move(180, 160, local.orientationError);
}

void PlayLogic::centerBeforeKick() {
  if (centerBeforeKickTimer < 2000) {
    int goalError = wrap180(local.frontGoal.coordinates.angle);
    int sideMoveAngle = goalError > 0 ? 90 : 270;

    drive.move(sideMoveAngle, 170, local.orientationError);
  }
  else {
    scoringState = ScoringState::AimAndKick;
    rotateAndKick();
  }
}
