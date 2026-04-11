#ifndef OPEN_MV_WAIT_FOR_START_BYTE_STATE_H
#define OPEN_MV_WAIT_FOR_START_BYTE_STATE_H

#include "CamerasStream.h"
#include <memory>

class OpenMVWaitForStartByteState : public CameraStreamState {
  public:
    OpenMVWaitForStartByteState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t portData) override {
        if (portData != START_BYTE_HIGH) return false;
        camerasStream.packet[0] = portData;
        camerasStream.changeOpenMVStreamState(std::make_unique<OpenMVWaitForSecondByteState>(camerasStream));
        return false;
    }
};

#endif