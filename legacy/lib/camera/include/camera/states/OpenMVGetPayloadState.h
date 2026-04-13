/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 *
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
 **/

#ifndef OPEN_MV_GET_PAYLOAD_STATE_H
#define OPEN_MV_GET_PAYLOAD_STATE_H

#include "CameraStreamState.h"

class OpenMVGetPayloadState : public CameraStreamState {
public:
  OpenMVGetPayloadState(CameraStream &cameraStream);

  bool execute(uint8_t &portData) override;
};

#endif
