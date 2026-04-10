#include "WaitForStartByteState.h"

bool WaitForStartByteState::execute(uint8_t &portData) {
    if (portData != START_BYTE_HIGH) return false;
    camerasStream.unitVIndex = 0;
    camerasStream.unitVChecksum = 0;
    camerasStream.changeState(new GetPayloadState(camerasStream));
    return false;
}