#ifndef UNIT_V_VERIFY_CHECKSUM_STATE_H
#define UNIT_V_VERIFY_CHECKSUM_STATE_H

#include "CameraStreamState.h"

class UnitVVerifyChecksumState : public CameraStreamState {
  public:
    UnitVVerifyChecksumState(CamerasStream &cameraStream) : CameraStreamState(cameraStream) {}

    bool execute(uint8_t &portData) override {
      if (portData == cameraStream.checksum) {
          cameraStream.changeState(std::make_unique<UnitVWaitForSecondStartByteState>(cameraStream));
          return false;
      }

      cameraStream.changeState(std::make_unique<UnitVWaitForStartByteState>(cameraStream));
      return false;
    }
};

#endif