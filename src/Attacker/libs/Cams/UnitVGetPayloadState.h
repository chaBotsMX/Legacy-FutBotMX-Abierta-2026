#ifndef UNIT_V_GET_PAYLOAD_STATE_H
#define UNIT_V_GET_PAYLOAD_STATE_H

#include "CameraStreamState.h"
#include <memory>

class UnitVGetPayloadState : public CameraStreamState {
  public:
    UnitVGetPayloadState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t &portData) override {
      camerasStream.unitVBuffer[camerasStream.unitVIndex++] = portData;
      camerasStream.unitVChecksum = (camerasStream.unitVChecksum + portData) & 0xFF;
      if (camerasStream.unitVIndex < 4) return false;
      camerasStream.changeState(std::make_unique<UnitVVerifyChecksumState>(camerasStream));
      return false;
    }
};

#endif