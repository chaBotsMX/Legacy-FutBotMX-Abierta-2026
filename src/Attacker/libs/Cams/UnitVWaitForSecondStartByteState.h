#ifndef UNIT_V_WAIT_FOR_SECOND_START_BYTE_STATE_H
#define UNIT_V_WAIT_FOR_SECOND_START_BYTE_STATE_H

#include "CameraStreamState.h"
#include <memory>

class UnitVWaitForSecondStartByteState : public CameraStreamState {
  public:
    UnitVWaitForSecondStartByteState(CamerasStream &camerasStream) : CameraStreamState(camerasStream) {}

    bool execute(uint8_t &portData) override {
      if (portData != START_BYTE_HIGH) {
          camerasStream.changeState(std::make_unique<UnitVWaitForStartByteState>(camerasStream));
          return false;
      }

      camerasStream.x = (int16_t)(unitVBuffer[0] | (unitVBuffer[1] << 8));
      camerasStream.y = (int16_t)(unitVBuffer[2] | (unitVBuffer[3] << 8));
      camerasStream.changeState(std::make_unique<UnitVWaitForStartByteState>(camerasStream));
      return true;
    }
};

#endif