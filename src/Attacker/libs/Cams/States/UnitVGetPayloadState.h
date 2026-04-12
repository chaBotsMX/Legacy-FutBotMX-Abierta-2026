/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 * 
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
**/

#ifndef UNIT_V_GET_PAYLOAD_STATE_H
#define UNIT_V_GET_PAYLOAD_STATE_H

#include "CameraStreamState.h"

class UnitVGetPayloadState : public CameraStreamState {
  public:
    UnitVGetPayloadState(CameraStream &cameraStream) : CameraStreamState(cameraStream) {}

    bool execute(uint8_t &portData) override {
      cameraStream.buffer[cameraStream.index++] = portData;
      cameraStream.checksum = (cameraStream.checksum + portData) & 0xFF;
      if (cameraStream.index < 4) return false;
      cameraStream.changeState(std::make_unique<UnitVVerifyChecksumState>(cameraStream));
      return false;
    }
};

#endif