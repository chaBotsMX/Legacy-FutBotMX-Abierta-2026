#ifndef UNIT_V_WAIT_FOR_START_BYTE_STATE_H
#define UNIT_V_WAIT_FOR_START_BYTE_STATE_H

#include "CameraStreamState.h"
#include <memory>

class UnitVWaitForStartByteState : public CameraStreamState {
  public:
    UnitVWaitForStartByteState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t &portData) override {
      if (portData != START_BYTE_HIGH) return false;
      camerasStream.unitVIndex = 0;
      camerasStream.unitVChecksum = 0;
      camerasStream.changeState(std::make_unique<UnitVGetPayloadState>(camerasStream));
      return false;
    }
};

#endif