int camX = 0;
int camY = 0;
int ballAng = 0;

bool readCameraXY(int &x, int &y) {
  const uint8_t START_BYTE = 0xAA;
  const uint8_t END_BYTE   = 0x55;

  while (Serial4.available() >= 7) {
    if (Serial4.read() != START_BYTE) {
      continue;
    }

    uint8_t data[6];
    if (Serial4.readBytes(data, 6) != 6) {
      return false;
    }

    if (data[5] != END_BYTE) {
      return false;
    }

    uint8_t checksum = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
    if (checksum != data[4]) {
      return false;
    }

    x = (int16_t)(data[0] | (data[1] << 8));
    y = (int16_t)(data[2] | (data[3] << 8));
    return true;
  }

  return false;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial4.begin(115200);
  Serial5.begin(115200);
  pinMode(13,OUTPUT);
}

void loop() {

    if(readCameraXY(camX, camY)){  
      digitalWrite(13,HIGH);
    /*Serial.print("X: ");
    Serial.print(camX);
    Serial.print(" Y: ");
    Serial.println(camY);*/
    Serial.print(" angulo: ");
    if(camX != 500){
    ballAng = atan2(camX,camY)* 180 / 3.1415;
    if (ballAng < 0){
      ballAng = abs(ballAng);
    }
    else{
      ballAng = 360 -ballAng;
    }
    }
    else{ballAng = 500;}
    Serial.println(ballAng);
  }
  Serial5.write(ballAng/2);
  // put your main code   here, to run repeatedly:
}
