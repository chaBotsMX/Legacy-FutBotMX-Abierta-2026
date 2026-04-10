/**
 * @file CamerasStream.cpp
 * @brief Library for handling camera communication.
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
**/

#include "CamerasStream.h"

void CamerasStream::begin(unsigned long baud) {
  port.begin(baud);
}

bool CamerasStream::updatePacketUnitV(int &x, int &y) {
  while (port.available()) {
    uint8_t portData = port.read();
    if(unitVStreamState->execute(portData)) return true;
  }

  return false;
}

bool CamerasStream::updatePacketOpenMV(int &x, int &y, int &goalX, int &goalY, int &goalColor) {
  while (port.available()) {
    uint8_t portData = port.read();

    switch (openMVState) {
      case 0:
        if (portData != START_BYTE_HIGH) break;
        packet[0] = portData;
        openMVState = 1;
        break;

      case 1:
        if (portData == START_BYTE_LOW) {
          packet[1] = portData;
          openMVIndex = 2;
          openMVState = 2;
        } 
        else if (portData == START_BYTE_HIGH) {
          packet[0] = portData;
          openMVState = 1;
        }
        else openMVState = 0;
        break;

      case 2:
        packet[openMVIndex++] = portData;

        if (openMVIndex >= PACKET_SIZE) {
          uint8_t checksum = 0;
          for (uint8_t i = 2; i <= 10; i++) {
            checksum ^= packet[i];
          }

          openMVState = 0;

          if (checksum == packet[11]) {
            y = ((uint16_t)packet[2] << 8) | packet[3];
            x = ((uint16_t)packet[4] << 8) | packet[5];

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
