/**
 * @file CamerasStream.cpp
 * @brief Library for handling camera communication.
 *
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
 **/

#ifndef OPENMV_STREAM_H
#define OPENMV_STREAM_H

#include "camera/CameraStream.h"
#include "camera/states/OpenMVWaitForStartByteState.h"

#define OPENMV_BUFFER_SIZE 12

class OpenMVStream : public CameraStream {
public:
  OpenMVStream(HardwareSerial &port) : CameraStream(port) {
    buffer.resize(OPENMV_BUFFER_SIZE);
    streamState = std::make_unique<OpenMVWaitForStartByteState>(*this);
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
