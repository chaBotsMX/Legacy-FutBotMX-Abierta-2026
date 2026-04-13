/**
 * @file CamerasStream.cpp
 * @brief Library for handling camera communication.
 *
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 06/04/26
 **/

#ifndef CAMERA_STREAM_H
#define CAMERA_STREAM_H

#include <Arduino.h>
#include <memory>
#include <vector>

#define BALL_OUT_OF_RANGE 500

class CameraStreamState;

class CameraStream {
public:
  CameraStream(HardwareSerial &port) : port(port) {}
  virtual ~CameraStream();

  void begin(unsigned long baud = 115200) { port.begin(baud); }

  virtual bool update() = 0;

  uint8_t START_BYTE_HIGH = 0xAA;
  uint8_t START_BYTE_LOW = 0x55;

  int ballX = BALL_OUT_OF_RANGE;
  int ballY = BALL_OUT_OF_RANGE;
  int goalX = BALL_OUT_OF_RANGE;
  int goalY = BALL_OUT_OF_RANGE;
  int goalColor = -1;

  uint8_t index = 0;
  uint8_t checksum = 0;
  std::vector<uint8_t> buffer;

  void changeState(std::unique_ptr<CameraStreamState> streamState) {
    this->streamState = std::move(streamState);
  }

protected:
  HardwareSerial &port;

  std::unique_ptr<CameraStreamState> streamState;
};

#endif
