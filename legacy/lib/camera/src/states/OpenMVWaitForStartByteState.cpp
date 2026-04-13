#include "camera/states/OpenMVWaitForStartByteState.h"
#include "camera/CameraStream.h"
#include "camera/states/OpenMVWaitForSecondStartByteState.h"

OpenMVWaitForStartByteState::OpenMVWaitForStartByteState(
    CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool OpenMVWaitForStartByteState::execute(uint8_t &portData) {
  if (portData != START_BYTE_HIGH)
    return false;
  cameraStream.buffer[0] = portData;
  cameraStream.changeState(
      std::make_unique<OpenMVWaitForSecondStartByteState>(cameraStream));
  return false;
}
