#include "camera/states/OpenMVGetPayloadState.h"
#include "camera/CameraStream.h"
#include "camera/states/OpenMVWaitForStartByteState.h"

OpenMVGetPayloadState::OpenMVGetPayloadState(CameraStream &cameraStream)
    : CameraStreamState(cameraStream) {}

bool OpenMVGetPayloadState::execute(uint8_t &portData) {
  cameraStream.buffer[cameraStream.index++] = portData;

  if (cameraStream.index < cameraStream.buffer.size())
    return false;

  uint8_t checksum = 0;
  for (uint8_t i = 2; i <= 10; i++) {
    checksum ^= cameraStream.buffer[i];
  }

  if (checksum != cameraStream.buffer[11]) {
    cameraStream.changeState(
        std::make_unique<OpenMVWaitForStartByteState>(cameraStream));
    return false;
  }

  cameraStream.ballX =
      ((uint16_t)cameraStream.buffer[2] << 8) | cameraStream.buffer[3];
  cameraStream.ballY =
      ((uint16_t)cameraStream.buffer[4] << 8) | cameraStream.buffer[5];

  cameraStream.goalX =
      ((uint16_t)cameraStream.buffer[6] << 8) | cameraStream.buffer[7];
  cameraStream.goalY =
      ((uint16_t)cameraStream.buffer[8] << 8) | cameraStream.buffer[9];
  cameraStream.goalColor = cameraStream.buffer[10];

  cameraStream.changeState(
      std::make_unique<OpenMVWaitForStartByteState>(cameraStream));
  return true;
}
