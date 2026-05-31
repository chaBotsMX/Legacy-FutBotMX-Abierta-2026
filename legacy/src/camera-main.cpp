#include "CameraInstance/CameraInstance.h"

CameraInstance robot;
void setup() {
  robot.init();
}


void loop() {
  robot.update();
}


