#ifndef UNIT_V_STREAM_H
#define UNIT_V_STREAM_H

#include "CameraStream.h"

#define BUFFER_SIZE 4

class UnitVStream : public CameraStream {
public:
    UnitVStream(HardwareSerial &port) : CameraStream(port), buffer(BUFFER_SIZE), streamState(std::make_unique<UnitVWaitForStartByteState>(*this)) {}

    bool update() override {
        while (port.available()) {
            uint8_t portData = port.read();
            if (streamState->execute(portData)) return true;
        }

        return false;
    }
};

#endif