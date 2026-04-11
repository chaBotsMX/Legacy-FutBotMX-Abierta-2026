#ifndef UNIT_V_VERIFY_CHECKSUM_STATE_H
#define UNIT_V_VERIFY_CHECKSUM_STATE_H

#include "CameraStreamState.h"
#include <memory>

class UnitVVerifyChecksumState : public CameraStreamState {
  public:
    UnitVVerifyChecksumState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t &portData) override {
      if (portData == camerasStream.unitVChecksum) {
          camerasStream.changeState(std::make_unique<UnitVWaitForSecondStartByteState>(camerasStream));
          return false;
      }

      camerasStream.changeState(std::make_unique<UnitVWaitForStartByteState>(camerasStream));
      return false;
    }
};

#endif