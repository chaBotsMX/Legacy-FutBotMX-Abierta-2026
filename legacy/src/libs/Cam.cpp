/**
 * @file Cam.cpp
 * @brief Library for handling camera communication.
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
 */

#include "Cam.h"

Cam::Cam(HardwareSerial &port_) : port(port_) {}

void Cam::begin(unsigned long baud) {
  port.begin(baud);
}

bool Cam::updatePacketUnitV(int &x, int &y) {
  while (port.available()) {
    uint8_t c = port.read();

    switch (unitVState) {
      case 0: // Esperar START_BYTE
        if (c == 0xAA) {
          unitVIndex = 0;
          unitVChecksum = 0;
          unitVState = 1;
        }
        break;

      case 1: // Recibir 4 bytes de payload
        unitVBuffer[unitVIndex++] = c;
        unitVChecksum = (unitVChecksum + c) & 0xFF;
        if (unitVIndex >= 4) {
          unitVState = 2;
        }
        break;

      case 2: // Verificar checksum
        if (c == unitVChecksum) {
          unitVState = 3;
        } else {
          unitVState = 0;
        }
        break;

      case 3: // Verificar END_BYTE
        if (c == 0x55) {
          x = (int16_t)(unitVBuffer[0] | (unitVBuffer[1] << 8));
          y = (int16_t)(unitVBuffer[2] | (unitVBuffer[3] << 8));
          unitVState = 0;
          return true;
        } else {
          unitVState = 0;
        }
        break;
    }
  }

  return false;
}

bool Cam::updatePacketOpenMV(int &x, int &y, int &goalX, int &goalY, int &goalColor) {
  while (port.available()) {
    uint8_t c = port.read();

    switch (openMVState) {
      case 0:
        if (c == START_BYTE_1) {
          packet[0] = c;
          openMVState = 1;
        }
        break;

      case 1:
        if (c == START_BYTE_2) {
          packet[1] = c;
          openMVIndex = 2;
          openMVState = 2;
        } else if (c == START_BYTE_1) {
          packet[0] = c;
          openMVState = 1;
        } else {
          openMVState = 0;
        }
        break;

      case 2:
        packet[openMVIndex++] = c;

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
