#ifndef UTILS_H
#define UTILS_H


inline float wrap180f(float anglef) {
  while (anglef > 180.0f) anglef -= 360.0f;
  while (anglef <= -180.0f) anglef += 360.0f;
  return anglef;
}

inline int wrap180(int angle) {
  while (angle > 180) angle -= 360;
  while (angle < -180) angle += 360;
  return angle;
}

inline int wrap360(int angle) {
  while (angle < 0) angle += 360;
  while (angle >= 360) angle -= 360;
  return angle;
}


#endif
