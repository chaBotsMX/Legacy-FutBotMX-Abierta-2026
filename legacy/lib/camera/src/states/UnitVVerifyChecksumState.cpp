#include "camera/states/UnitVVerifyChecksumState.h"
#include "camera/CameraStream.h"
#include "camera/states/UnitVGetPayloadState.h"

UnitVVerifyChecksumState::UnitVVerifyChecksumState(CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool UnitVVerifyChecksumState::execute(uint8_t &portData) {
  if (portData != cameraStream.START_BYTE_HIGH)
    return false;
  cameraStream.index = 0;
  cameraStream.checksum = 0;
  cameraStream.changeState(
      std::make_unique<UnitVGetPayloadState>(cameraStream));
  return false;
}
