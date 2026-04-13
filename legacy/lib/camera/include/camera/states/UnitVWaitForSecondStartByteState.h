/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 *
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
 **/

#ifndef UNIT_V_WAIT_FOR_SECOND_START_BYTE_STATE_H
#define UNIT_V_WAIT_FOR_SECOND_START_BYTE_STATE_H

#include "camera/states/CameraStreamState.h"
#include "camera/states/UnitVWaitForStartByteState.h"

class UnitVWaitForSecondStartByteState : public CameraStreamState {
public:
  UnitVWaitForSecondStartByteState(CameraStream &cameraStream)
      : CameraStreamState(cameraStream) {}

  bool execute(uint8_t &portData) override {
    if (portData != START_BYTE_HIGH) {
      cameraStream.changeState(
          std::make_unique<UnitVWaitForStartByteState>(cameraStream));
      return false;
    }

    cameraStream.ballX =
        (int16_t)(cameraStream.buffer[0] | (cameraStream.buffer[1] << 8));
    cameraStream.ballY =
        (int16_t)(cameraStream.buffer[2] | (cameraStream.buffer[3] << 8));
    cameraStream.changeState(
        std::make_unique<UnitVWaitForStartByteState>(cameraStream));
    return true;
  }
};

#endif
