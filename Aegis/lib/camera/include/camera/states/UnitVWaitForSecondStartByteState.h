/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 *
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
 **/

#ifndef UNIT_V_WAIT_FOR_SECOND_START_BYTE_STATE_H
#define UNIT_V_WAIT_FOR_SECOND_START_BYTE_STATE_H

#include "camera/states/CameraStreamState.h"

class UnitVWaitForSecondStartByteState : public CameraStreamState {
public:
  UnitVWaitForSecondStartByteState(CameraStream &cameraStream);

  bool execute(uint8_t &portData) override;
};

#endif
