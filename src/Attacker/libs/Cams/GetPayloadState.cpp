#include "GetPayloadState.h"

bool GetPayloadState::execute(uint8_t &portData) {
    camerasStream.unitVBuffer[camerasStream.unitVIndex++] = portData;
    camerasStream.unitVChecksum = (camerasStream.unitVChecksum + portData) & 0xFF;
    if (camerasStream.unitVIndex < 4) return false;
    camerasStream.changeState(new VerifyChecksumState(camerasStream));
    return false;
}