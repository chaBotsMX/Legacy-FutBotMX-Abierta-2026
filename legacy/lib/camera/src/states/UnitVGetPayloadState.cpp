#include "camera/states/UnitVGetPayloadState.h"
#include "camera/CameraStream.h"
#include "camera/states/UnitVVerifyChecksumState.h"

UnitVGetPayloadState::UnitVGetPayloadState(CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool UnitVGetPayloadState::execute(uint8_t &portData) {
  cameraStream.buffer[cameraStream.index++] = portData;
  cameraStream.checksum = (cameraStream.checksum + portData) & 0xFF;
  if (cameraStream.index < cameraStream.buffer.size())
    return false;
  cameraStream.changeState(
      std::make_unique<UnitVVerifyChecksumState>(cameraStream));
  return false;
}
