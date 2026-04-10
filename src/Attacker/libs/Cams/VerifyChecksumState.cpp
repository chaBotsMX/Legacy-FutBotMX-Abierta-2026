#include "VerifyChecksumState.h"

bool VerifyChecksumState::execute(uint8_t &portData) {
    if (portData == camerasStream.unitVChecksum) {
        camerasStream.changeState(new WaitForSecondStartByteState(camerasStream));
        return false;
    }

    camerasStream.changeState(new WaitForStartByteState(camerasStream));
    return false;
}