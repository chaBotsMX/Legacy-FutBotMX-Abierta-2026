#include <Line.h>

Line Ls;

void setup() {
  Serial.begin(115200);
  Serial1.begin(2000000);

}

void loop() {

  Ls.update();
  Serial.print("  avoidAngle: ");
  Serial.print(Ls.getAvoidAngle());

  Serial1.write(Ls.getAvoidAngle() / 2);
}




