#include "UART.h"

void UART::init(){
    LINE.begin(2000000);
    OPENMVLEFT.begin(115200);
    OPENMVRIGHT.begin(115200);


}