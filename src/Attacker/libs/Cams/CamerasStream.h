/**
 * @file CamerasStream.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 07/04/26
 */

#ifndef CAMERAS_STREAM_H
#define CAMERAS_STREAM_H

#include "CameraStreamState.h"
#include <Arduino.h>
#include <memory>

#define START_BYTE_HIGH (uint8_t) 0xAA
#define START_BYTE_LOW (uint8_t) 0x55

#define BUFFER_SIZE 4
#define PACKET_SIZE 12
#define BALL_OUT_OF_RANGE 500

uint8_t packet[PACKET_SIZE];

class CamerasStream {
  public:
    Cams(HardwareSerial &port) : port(port) {}

    void begin(unsigned long baud = 115200);

    bool updatePacketOpenMV(int &x, int &y, int &goalX, int &goalY, int &goalColor);
    bool updatePacketUnitV(int &x, int &y);
    
  private:
    HardwareSerial &port;

    int ballX = BALL_OUT_OF_RANGE;
    int ballY = BALL_OUT_OF_RANGE;

    uint8_t unitVState = 0;
    uint8_t unitVBuffer[BUFFER_SIZE] = {0};
    uint8_t unitVIndex = 0;
    uint8_t unitVChecksum = 0;
    uint8_t openMVState = 0;
    uint8_t openMVIndex = 0;

    std::unique_ptr<CameraStreamState> unitVStreamState = std::make_unique<UnitVWaitForStartByteState>(*this);
    std::unique_ptr<CameraStreamState> openMVStreamState = std::make_unique<OpenMVWaitForStartByteState>(*this);

    void changeUnitVStreamState(std::unique_ptr<CameraStreamState> unitVStreamState);
    void changeOpenMVStreamState(std::unique_ptr<CameraStreamState> openMVStreamState);
};

#endif
