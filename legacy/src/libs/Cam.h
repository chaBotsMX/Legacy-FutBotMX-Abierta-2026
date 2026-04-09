/**
 * @file Cams.h
 * 
 * @author Roy Ivan Barron Martinez / chaBotsMX
 * @date 07/04/26
 */

#ifndef Cam_H
#define Cam_H

#include <Arduino.h>

class Cam {

  public:
    Cam(HardwareSerial &port_);
    void begin(unsigned long baud = 115200);

    bool updatePacketOpenMV(int &x, int &y, int &goalX, int &goalY, int &goalColor);
    bool updatePacketUnitV(int &x, int &y);
  private:
    HardwareSerial &port;
    int ballX = 500;
    int ballY = 500;  
    uint8_t unitVState = 0;
    uint8_t unitVBuffer[4] = {0};
    uint8_t unitVIndex = 0;
    uint8_t unitVChecksum = 0;
    uint8_t openMVState = 0;
    uint8_t openMVIndex = 0;
    static constexpr uint8_t START_BYTE_1 = 0xAA;
    static constexpr uint8_t START_BYTE_2 = 0x55;
    static constexpr uint8_t PACKET_SIZE  = 12;
    uint8_t packet[PACKET_SIZE];
    };

#endif
