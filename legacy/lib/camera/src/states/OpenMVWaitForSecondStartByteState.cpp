#include "camera/states/OpenMVWaitForSecondStartByteState.h"
#include "camera/CameraStream.h"
#include "camera/states/OpenMVGetPayloadState.h"
#include "camera/states/OpenMVWaitForStartByteState.h"

OpenMVWaitForSecondStartByteState::OpenMVWaitForSecondStartByteState(
    CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool OpenMVWaitForSecondStartByteState::execute(uint8_t &portData) {
  if (portData == cameraStream.START_BYTE_LOW) {
    cameraStream.buffer[1] = portData;
    cameraStream.index = 2;
    cameraStream.changeState(
        std::make_unique<OpenMVGetPayloadState>(cameraStream));
  } else if (portData == cameraStream.START_BYTE_HIGH)
    cameraStream.buffer[0] = portData;
  else
    cameraStream.changeState(
        std::make_unique<OpenMVWaitForStartByteState>(cameraStream));

  return false;
}
