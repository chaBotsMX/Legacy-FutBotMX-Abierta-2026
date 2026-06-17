#include "MainUart.h"

bool MainUart::receiveInfoFromLineTeensy(HardwareSerial &port) {
  while(port.available()){
    int rawAngle = port.read() * 2;
    if(rawAngle != OUT_OF_RANGE){
      receiveData.lineAng = rawAngle;
    }
    else{
      receiveData.lineAng = OUT_OF_RANGE;
    }

    return true;

  }
  return false;
}

bool MainUart::receiveInfoFromCameraTeensy(HardwareSerial &port) {
  static uint8_t state = 0;
  static Data incoming;
  static uint8_t checksum = 0;

  while (port.available()) {
    uint8_t byteIn = port.read();

    switch (state) {
      case 0: // Esperando START_BYTE
        if (byteIn == START_BYTE) {
          incoming = Data(); // limpia datos temporales
          checksum = 0;
          state = 1;
        }
        break;

      case 1: // ballAng / 2
        incoming.ballAng = byteIn * 2;
        checksum ^= byteIn;
        state = 2;
        break;

      case 2: // haveBall
        incoming.haveBall = byteIn == 1;
        checksum ^= byteIn;
        state = 3;
        break;

      case 3: // ballDistance / 2
        incoming.ballDistance = byteIn * 2;
        checksum ^= byteIn;
        state = 4;
        break;

      case 4: // front.goalX / 2
        incoming.frontGoal.x = byteIn * 2;
        checksum ^= byteIn;
        state = 5;
        break;

      case 5: // front.goalY / 2
        incoming.frontGoal.y = byteIn * 2;
        checksum ^= byteIn;
        state = 6;
        break;

      case 6: // back.goalX / 2
        incoming.backGoal.x = byteIn * 2;
        checksum ^= byteIn;
        state = 7;
        break;

      case 7: // back.goalY / 2
        incoming.backGoal.y = byteIn * 2;
        checksum ^= byteIn;
        state = 8;
        break;

      case 8: // checksum
        if (byteIn == checksum) {
          state = 9;
        } else {
          state = 0;

          if (byteIn == START_BYTE) {
            incoming = Data();
            checksum = 0;
            state = 1;
          }
        }
        break;

      case 9: // Esperando END_BYTE
        if (byteIn == END_BYTE) {
          receiveData.ballAng = incoming.ballAng;
          receiveData.haveBall = incoming.haveBall;
          receiveData.ballDistance = incoming.ballDistance;

          receiveData.frontGoal.x = incoming.frontGoal.x;
          receiveData.frontGoal.y = incoming.frontGoal.y;

          receiveData.backGoal.x = incoming.backGoal.x;
          receiveData.backGoal.y = incoming.backGoal.y;

          state = 0;
          return true;
        } else {
          state = 0;

          // Si este byte corrupto era realmente otro inicio de paquete,
          // no lo perdemos.
          if (byteIn == START_BYTE) {
            incoming = Data();
            checksum = 0;
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


void MainUart::updateUARTData(Data &uart){
    receiveInfoFromCameraTeensy(Serial4);
    receiveInfoFromLineTeensy(Serial3); 
    uart.ballAng  = receiveData.ballAng;
    uart.haveBall =  receiveData.haveBall;  
    uart.ballDistance =  receiveData.ballDistance;  

    uart.frontGoal.x = receiveData.frontGoal.x;  
    uart.frontGoal.y = receiveData.frontGoal.y;  

    uart.backGoal.x  = receiveData.backGoal.x; 
    uart.backGoal.y = receiveData.backGoal.y;  
    uart.lineAng = receiveData.lineAng; 
}

void MainUart::init(){
    Serial4.begin(2000000);
    delay(1000);
    Serial3.begin(2000000);
    delay(1000);

}
