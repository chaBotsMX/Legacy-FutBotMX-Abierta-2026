#ifndef UNIT_V_WAIT_FOR_START_BYTE_STATE_H
#define UNIT_V_WAIT_FOR_START_BYTE_STATE_H

#include "CameraStreamState.h"

class UnitVWaitForStartByteState : public CameraStreamState {
  public:
    UnitVWaitForStartByteState(CameraStream &cameraStream) : CameraStreamState(cameraStream) {}

    bool execute(uint8_t &portData) override {
      if (portData != START_BYTE_HIGH) return false;
      cameraStream.index = 0;
      cameraStream.checksum = 0;
      cameraStream.changeState(std::make_unique<UnitVGetPayloadState>(cameraStream));
      return false;
    }
};

#endif