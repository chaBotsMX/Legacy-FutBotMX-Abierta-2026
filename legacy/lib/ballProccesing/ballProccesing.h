#ifndef BALLPROCCESING_H
#define BALLPROCCESING_H


#include "../utils/utils.h"

#define INVALID_DATA 500
#define DRIBBLER_AREA_X_MIN 140
#define DRIBBLER_AREA_X_MAX 200
#define DRIBBLER_AREA_Y_MAX 15

#define QVGA_WIDTH 320
#define QVGA_HEIGHT 240

// verifica si la pelota esta en el area del dribbler para saber si la capturo y a si patearla o encender el dribbler 
inline bool haveBall(int verifyX, int verifyY) {
  if(verifyX != INVALID_DATA) {
    if(verifyX > DRIBBLER_AREA_X_MIN && verifyX < DRIBBLER_AREA_X_MAX && verifyY < DRIBBLER_AREA_Y_MAX){
      return true;
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
}

inline int getGoalAngle(int inputX, int inputY) {
  int resultAng = 500;
  if(inputX != INVALID_DATA && inputY != INVALID_DATA){
    resultAng = atan2(inputY - (QVGA_HEIGHT / 2),inputX - (QVGA_WIDTH / 2)) * 180 / PI;
    return wrap360(resultAng - 90);
  }
  return 500;
}



inline int getBallAngle(int inputX, int inputY, int side ) {
  int resultAng = 500;
  switch (side) {

    case 0: // frente

        resultAng =atan2(inputY + 40,inputX - (QVGA_WIDTH / 2)) * 180 / PI;
        return wrap360(resultAng - 90);
  
    case 1: //atras

        resultAng = atan2(inputY + 40,inputX - (QVGA_WIDTH / 2)) * 180 / PI;
        return wrap360(resultAng + 90);
  
    case 2: // izquierda
        resultAng = atan2(inputY,inputX) * 180 / PI;
        return wrap360(resultAng + 180);
  
    case 3: // derecha
        resultAng = atan2(inputY,inputX) * 180 / PI;
        return wrap360(resultAng );
    default:
        return 500;
    }
}


inline int getBallDistance(int inputX, int inputY,int side ) {
  switch (side) {
    case 0: // frente y atras
        return sqrt(pow(inputX - (QVGA_WIDTH / 2), 2) + pow(inputY + 40, 2));
    case 1: //derecho e izquierdo
        return sqrt(pow(inputX, 2) + pow(inputY, 2));
  }
  return 500;
}
#endif