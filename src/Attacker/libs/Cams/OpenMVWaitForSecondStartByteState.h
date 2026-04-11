#ifndef OPEN_MV_WAIT_FOR_SECOND_START_BYTE_STATE_H
#define OPEN_MV_WAIT_FOR_SECOND_START_BYTE_STATE_H

#include "CamerasStream.h"
#include <memory>

class OpenMVWaitForSecondByteState : public CameraStreamState {
  public:
    OpenMVWaitForSecondByteState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t portData) override {
        if (portData == START_BYTE_LOW) {
          camerasStream.packet[1] = portData;
          camerasStream.openMVIndex = 2;
          camerasStream.changeOpenMVStreamState(std::make_unique<OpenMVGetPayloadState>(camerasStream));
        } 
        else if (portData == START_BYTE_HIGH) camerasStream.packet[0] = portData;
        else camerasStream.changeOpenMVStreamState(std::make_unique<OpenMVWaitForStartByteState>(camerasStream));
        
        return false;
    }
};

#endif