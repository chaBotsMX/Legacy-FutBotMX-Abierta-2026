#ifndef TRYGONOMETRY_H
#define TRYGONOMETRY_H

#include "dataTypes.h"
#include "utils.h"
#include <cmath>
#include <Arduino.h>
inline int getEuclideanDistance(int x, int y) {
  if (x == 500 || y == 500) {
    return 500; // No hay información válida
  }
  return sqrt(pow(x - 160, 2) + pow(y - 120, 2));
}

inline Vector sumVectors(Vector &v1, Vector &v2){
    if (v1.angle == 500 || v1.magnitude == 500 || v2.angle == 500 || v2.magnitude == 500) {
    return {500,500}; // No hay información válida
  }
  int x = v1.magnitude * cos(radians(v1.angle)) + v2.magnitude * cos(radians(v2.angle));
  int y = v1.magnitude * sin(radians(v1.angle)) + v2.magnitude * sin(radians(v2.angle));
  int resultAngle = atan2(y, x) * 180 / PI;
  return   {wrap360(resultAngle), (int)sqrt(x * x + y * y)};
}

#endif // TRYGONOMETRY_H