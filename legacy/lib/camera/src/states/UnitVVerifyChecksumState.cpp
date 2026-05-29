#include "camera/states/UnitVVerifyChecksumState.h"
#include "camera/CameraStream.h"
#include "camera/states/UnitVWaitForStartByteState.h"

UnitVVerifyChecksumState::UnitVVerifyChecksumState(CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool UnitVVerifyChecksumState::execute(uint8_t &portData) {

  if (portData != cameraStream.START_BYTE_LOW) {
    cameraStream.changeState(
        std::make_unique<UnitVWaitForStartByteState>(cameraStream));
    return false;
  }

  cameraStream.ballX =
      (int16_t)(cameraStream.buffer[0] | (cameraStream.buffer[1] << 8));

  cameraStream.ballY =
      (int16_t)(cameraStream.buffer[2] | (cameraStream.buffer[3] << 8));

  cameraStream.changeState(
      std::make_unique<UnitVWaitForStartByteState>(cameraStream));

  return true;
} 