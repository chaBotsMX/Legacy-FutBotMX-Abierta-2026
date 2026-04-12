/**
 * @file CamerasStream.cpp
 * @brief Library for handling camera communication.
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
**/

#ifndef OPENMV_STREAM_H
#define OPENMV_STREAM_H

#include "CameraStream.h"

#define BUFFER_SIZE 12

class OpenMVStream : public CameraStream {
public:
    OpenMVStream(HardwareSerial &port) : CameraStream(port), buffer(BUFFER_SIZE), streamState(std::make_unique<OpenMvWaitForStartByteState>(*this)) {}

    bool update() override {
        while (port.available()) {
            uint8_t portData = port.read();
            if (streamState->execute(portData)) return true;
        }

        return false;
    }
};

#endif