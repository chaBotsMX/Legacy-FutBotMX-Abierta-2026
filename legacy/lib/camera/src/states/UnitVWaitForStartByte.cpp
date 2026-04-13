#include "camera/CameraStream.h"
#include "camera/states/UnitVGetPayloadState.h"
#include "camera/states/UnitVWaitForStartByteState.h"

UnitVWaitForStartByteState::UnitVWaitForStartByteState(
    CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool UnitVWaitForStartByteState::execute(uint8_t &portData) {
  if (portData != cameraStream.START_BYTE_HIGH)
    return false;
  cameraStream.index = 0;
  cameraStream.checksum = 0;
  cameraStream.changeState(
      std::make_unique<UnitVGetPayloadState>(cameraStream));
  return false;
}
