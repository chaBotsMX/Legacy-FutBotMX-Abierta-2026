/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 * 
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
**/

#ifndef OPEN_MV_WAIT_FOR_START_BYTE_STATE_H
#define OPEN_MV_WAIT_FOR_START_BYTE_STATE_H

#include "CameraStream.h"

class OpenMVWaitForStartByteState : public CameraStreamState {
  public:
    OpenMVWaitForStartByteState(CameraStream &cameraStream) : CameraStreamState(cameraStream) {}

    bool execute(uint8_t portData) override {
        if (portData != START_BYTE_HIGH) return false;
        cameraStream.buffer[0] = portData;
        cameraStream.changeOpenMVStreamState(std::make_unique<OpenMVWaitForSecondByteState>(camerasStream));
        return false;
    }
};

#endif