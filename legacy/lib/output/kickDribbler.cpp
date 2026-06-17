#include "kickDribbler.h"

void kickDribbler::sendKick(){
  static elapsedMillis kickTimeOut = 0;
  if(kickTimeOut > 500){
    Serial4.write(1);
    kickTimeOut = 0;
  }
}

void kickDribbler::turnDribblerOn(){
    Serial4.write(2);
}

void kickDribbler::turnDribblerOff(){
    Serial4.write(3);
}