#ifndef OPEN_MV_GET_PAYLOAD_STATE_H
#define OPEN_MV_GET_PAYLOAD_STATE_H

#include "CameraStreamState.h"
#include <memory>

class OpenMVGetPayloadState : public CameraStreamState {
  public:
    OpenMVGetPayloadState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t portData) override {
      camerasStream.packet[camerasStream.openMVIndex++] = portData;

      if (camerasStream.openMVIndex < PACKET_SIZE) return false;

      uint8_t checksum = 0;
      for (uint8_t i = 2; i <= 10; i++) {
        checksum ^= camerasStream.packet[i];
      }

      camerasStream.changeOpenMVStreamState(std::make_unique<OpenMVWaitForStartByteState>(camerasStream));

      if (checksum != camerasStream.packet[11]) return false;

      camerasStream.x = ((uint16_t)camerasStream.packet[2] << 8) | camerasStream.packet[3];
      camerasStream.y = ((uint16_t)camerasStream.packet[4] << 8) | camerasStream.packet[5];

      camerasStream.goalX = ((uint16_t)camerasStream.packet[6] << 8) | camerasStream.packet[7];
      camerasStream.goalY = ((uint16_t)camerasStream.packet[8] << 8) | camerasStream.packet[9];
      camerasStream.goalColor = camerasStream.packet[10];

      return true;
    }
};

#endif