#include "WaitForSecondStartByteState.h"

bool WaitForSecondStartByteState::execute(uint8_t &portData) {
    if (portData != START_BYTE_HIGH) {
        camerasStream.changeState(new WaitForStartByteState(camerasStream));
        return false;
    }

    camerasStream.x = (int16_t)(unitVBuffer[0] | (unitVBuffer[1] << 8));
    camerasStream.y = (int16_t)(unitVBuffer[2] | (unitVBuffer[3] << 8));
    camerasStream.changeState(new WaitForStartByteState(camerasStream));
    return true;
}