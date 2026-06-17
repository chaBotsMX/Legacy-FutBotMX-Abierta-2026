#ifndef MAINUART_H
#define MAINUART_H

#include "../../../utils/definitions.h"
#include "../../../utils/dataTypes.h"
#include "HardwareSerial.h"
#include "../../../utils/utils.h"

class MainUart {
    public:
        void init();
        void updateUARTData(Data &uart);
    private:
        bool receiveInfoFromLineTeensy(HardwareSerial &port);
        bool receiveInfoFromCameraTeensy(HardwareSerial &port);
        Data receiveData;
};


#endif