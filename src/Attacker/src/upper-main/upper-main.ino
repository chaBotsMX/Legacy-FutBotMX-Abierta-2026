int ballAng = 0;
const uint8_t START_BYTE_1 = 0xAA;
const uint8_t START_BYTE_2 = 0x55;
const uint8_t PACKET_SIZE  = 12;

uint8_t packet[PACKET_SIZE];

int16_t ballX = 2000;
int16_t ballY = 2000;
int16_t goalX = 2000;
int16_t goalY = 2000;
int8_t goalColor = 0;


bool readPacket() {
  static uint8_t state = 0;
  static uint8_t index = 0;

  while (Serial4.available()) {
    uint8_t c = Serial4.read();

    switch (state) {
      case 0:
        if (c == START_BYTE_1) {
          packet[0] = c;
          state = 1;
        }
        break;

      case 1:
        if (c == START_BYTE_2) {
          packet[1] = c;
          index = 2;
          state = 2;
        } else if (c == START_BYTE_1) {
          packet[0] = c;
          state = 1;
        } else {
          state = 0;
        }
        break;

      case 2:
        packet[index++] = c;

        if (index >= PACKET_SIZE) {
          uint8_t checksum = 0;
          for (uint8_t i = 2; i <= 10; i++) {
            checksum ^= packet[i];
          }

          state = 0;

          if (checksum == packet[11]) {
            ballX = ((uint16_t)packet[2] << 8) | packet[3];
            ballY = ((uint16_t)packet[4] << 8) | packet[5];
            goalX = ((uint16_t)packet[6] << 8) | packet[7];
            goalY = ((uint16_t)packet[8] << 8) | packet[9];
            goalColor = packet[10];
            return true;
          }
        }
        break;
    }
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

    if(readPacket()){  
      digitalWrite(13,HIGH);
    Serial.print("X: ");
    Serial.print(ballX);
    Serial.print(" Y: ");
    Serial.println(ballY);
    Serial.print(" angulo: ");
    if(ballX != 2000){
    ballX = ballX - 160;
    //Serial.print(ballX);
    ballAng = atan2(ballX,ballY)* 180 / 3.1415;
    if (ballAng < 0){
      ballAng = abs(ballAng);
    }
    else{
      ballAng = 360 -ballAng;
    }
    }
    else{ballAng = 500;}
    if(ballX < 180 - 160 && ballX > 130 - 160&& ballY < 20){
      ballAng = 0;
    }
    Serial.println(ballAng);

  }
  Serial5.write(ballAng/2);

}
