#ifndef VERIFY_CHECKSUM_STATE_H
#define VERIFY_CHECKSUM_STATE_H

#include "CameraStreamState.h"

class VerifyChecksumState : public CameraStreamState {
  public:
    VerifyChecksumState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t &portData) override;
};

#endif