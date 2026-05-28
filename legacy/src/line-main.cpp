#include "Line.h"

Line Ls;

void setup() {
  Serial.begin(115200);
  Serial3.begin(2000000);

}

void loop() {
  
  Ls.update();
 // Serial.print("  avoidAngle: ");
  Serial.println(Ls.getAvoidAngle());
  
  Serial3.write(Ls.getAvoidAngle() / 2);
}



