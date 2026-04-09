#include <Line.h>

Line LineSensor;

void setup() {
  Serial.begin(115200);
  Serial1.begin(2000000);
}

void loop() {

  LineSensor.update();
  Serial.print("  avoidAngle: ");
  Serial.print(LineSensor.getAvoidAngle());

  Serial1.write(LineSensor.getAvoidAngle() / 2);
}




