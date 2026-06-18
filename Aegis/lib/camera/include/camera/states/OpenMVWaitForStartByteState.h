/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 *
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
 **/

#ifndef OPEN_MV_WAIT_FOR_START_BYTE_STATE_H
#define OPEN_MV_WAIT_FOR_START_BYTE_STATE_H

#include "CameraStreamState.h"

class OpenMVWaitForStartByteState : public CameraStreamState {
public:
  OpenMVWaitForStartByteState(CameraStream &cameraStream);

  bool execute(uint8_t &portData) override;
};

#endif
