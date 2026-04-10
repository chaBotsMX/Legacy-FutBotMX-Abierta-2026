#ifndef WAIT_FOR_SECOND_START_BYTE_STATE_H
#define WAIT_FOR_SECOND_START_BYTE_STATE_H

#include "CameraStreamState.h"

class WaitForSecondStartByteState : public CameraStreamState {
  public:
    WaitForSecondStartByteState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t &portData) override;
};

#endif