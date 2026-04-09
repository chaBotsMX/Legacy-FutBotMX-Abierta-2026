/**
 * @file Cams.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 07/04/26
 */

#ifndef Cams_H
#define Cams_H

#include <Arduino.h>

#define START_BYTE_HIGH (uint8_t) 0xAA
#define START_BYTE_LOW (uint8_t) 0x55

#define PACKET_SIZE 12

#define BALL_OUT_OF_RANGE 500

uint8_t packet[PACKET_SIZE];

class Cams {
  public:
    Cams(HardwareSerial &port_);

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
};

#endif
