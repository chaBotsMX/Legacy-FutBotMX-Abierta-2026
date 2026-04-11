/**
 * @file CamerasStream.cpp
 * @brief Library for handling camera communication.
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
**/

#ifndef CAMERA_STREAM_H
#define CAMERA_STREAM_H

#include "CameraStreamState.h"
#include <Arduino.h>
#include <memory>
#include <vector>

#define START_BYTE_HIGH (uint8_t) 0xAA
#define START_BYTE_LOW (uint8_t) 0x55

#define BALL_OUT_OF_RANGE 500

class CameraStream {
public:
    CameraStream(HardwareSerial &port) : port(port) {}

    void begin(unsigned long baud = 115200) {
        port.begin(baud);
    }

    virtual void update() = 0;

private:
    HardwareSerial &port;

    int ballX = BALL_OUT_OF_RANGE;
    int ballY = BALL_OUT_OF_RANGE;
    int goalX = BALL_OUT_OF_RANGE;
    int goalY = BALL_OUT_OF_RANGE;
    int goalColor = -1;

    uint8_t index = 0;
    uint8_t checksum = 0;
    std::vector<uint8_t> buffer;

    std::unique_ptr<CameraStreamState> streamState;

    void changeState(std::unique_ptr<CameraStreamState> streamState) {
        this->streamState = std::move(streamState);
    }
};

#endif