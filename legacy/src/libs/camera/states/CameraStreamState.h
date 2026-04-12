/**
 * @file CameraStreamState.h
 * @brief Library for handling camera communication states.
 * 
 * @author Diego Vitales Medellin / chaBotsMX
 * @date 11/04/26
**/

#ifndef CAMERA_STREAM_STATE_H
#define CAMERA_STREAM_STATE_H

#include "../CameraStream.h"
#include <memory>

class CameraStreamState {
  public:
    CameraStreamState(CameraStream &cameraStream) : cameraStream(cameraStream) {}

  protected:
    CameraStream &cameraStream;
    virtual ~CameraStreamState() {}

    virtual bool execute(uint8_t &portData) = 0;
};

#endif