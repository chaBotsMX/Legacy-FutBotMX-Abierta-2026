#ifndef OPEN_MV_WAIT_FOR_SECOND_START_BYTE_STATE_H
#define OPEN_MV_WAIT_FOR_SECOND_START_BYTE_STATE_H

#include "CameraStream.h"
#include <memory>

class OpenMVWaitForSecondByteState : public CameraStreamState {
  public:
    OpenMVWaitForSecondByteState(CameraStream &cameraStream) : CameraStreamState(cameraStream) {}

    bool execute(uint8_t portData) override {
        if (portData == START_BYTE_LOW) {
          cameraStream.buffer[1] = portData;
          cameraStream.index = 2;
          cameraStream.changeOpenMVStreamState(std::make_unique<OpenMVGetPayloadState>(cameraStream));
        } 
        else if (portData == START_BYTE_HIGH) cameraStream.buffer[0] = portData;
        else cameraStream.changeOpenMVStreamState(std::make_unique<OpenMVWaitForStartByteState>(cameraStream));
        
        return false;
    }
};

#endif