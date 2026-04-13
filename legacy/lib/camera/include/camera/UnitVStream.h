/**
 * @file CamerasStream.cpp
 * @brief Library for handling camera communication.
 *
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
 **/

#ifndef UNIT_V_STREAM_H
#define UNIT_V_STREAM_H

#include "camera/CameraStream.h"
#include "camera/states/UnitVWaitForStartByteState.h"

#define UNIT_V_BUFFER_SIZE 4

class UnitVStream : public CameraStream {
public:
  UnitVStream(HardwareSerial &port) : CameraStream(port) {
    buffer.resize(UNIT_V_BUFFER_SIZE);
    streamState = std::make_unique<UnitVWaitForStartByteState>(*this);
  }

  bool update() {
    while (port.available()) {
      uint8_t portData = port.read();
      if (streamState->execute(portData))
        return true;
    }

    return false;
  }
};

#endif
