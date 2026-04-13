/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 *
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
 **/

#ifndef UNIT_V_GET_PAYLOAD_STATE_H
#define UNIT_V_GET_PAYLOAD_STATE_H

#include "camera/states/CameraStreamState.h"

class UnitVGetPayloadState : public CameraStreamState {
public:
  UnitVGetPayloadState(CameraStream &cameraStream);

  bool execute(uint8_t &portData) override;
};

#endif
