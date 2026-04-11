/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 * 
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
**/

#ifndef CAMERA_STREAM_STATE_H
#define CAMERA_STREAM_STATE_H

#include "CamerasStream.h"

class CameraStreamState {
  public:
    CameraStreamState(CamerasStream &camerasStream) : camerasStream(camerasStream) {}

  protected:
    CamerasStream &camerasStream;
    virtual ~CameraStreamState() {}

    virtual bool execute(uint8_t &portData) = 0;
};

#endif