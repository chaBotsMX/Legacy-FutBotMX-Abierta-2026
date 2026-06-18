#include "camera/states/UnitVWaitForsecondStartByteState.h"
#include "camera/CameraStream.h"
#include "camera/states/UnitVWaitForStartByteState.h"

UnitVWaitForSecondStartByteState::UnitVWaitForSecondStartByteState(
    CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool UnitVWaitForSecondStartByteState::execute(uint8_t &portData) {
  if (portData != cameraStream.START_BYTE_HIGH) {
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
