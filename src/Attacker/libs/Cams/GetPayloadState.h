#ifndef GET_PAYLOAD_STATE_H
#define GET_PAYLOAD_STATE_H

#include "CameraStreamState.h"

class GetPayloadState : public CameraStreamState {
  public:
    GetPayloadState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t portData) override;
};

#endif