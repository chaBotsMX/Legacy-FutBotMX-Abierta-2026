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
    if(openMVStreamState->execute(portData)) return true;
  }

  return false;
}
