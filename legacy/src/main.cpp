//La pcb superior que manda anuglo esta conectada al Serial3
//la pcb de linea al seral4
#include "MainInstance/MainInstance.h"

MainInstance mainInstance;

void setup()
{  
  mainInstance.init();
}

void loop() 
{
  
  mainInstance.update();
}
