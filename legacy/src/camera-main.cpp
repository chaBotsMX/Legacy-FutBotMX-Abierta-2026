#include "CameraInstance/CameraInstance.h"

CameraInstance robot;
void setup() {
  delay(3000);
  robot.init();
}


void loop() {
  robot.update();
}


