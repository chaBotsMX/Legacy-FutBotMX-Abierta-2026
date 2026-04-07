#ifndef ANGLE_UTILS_H
#define ANGLE_UTILS_H

namespace AngleUtils {
  constexpr float kPi = 3.14159265f;
  constexpr float kRadToDeg = 180.0f / kPi;
  constexpr float kDegToRad = kPi / 180.0f;

  inline float radToDeg(float radians) {
    return radians * kRadToDeg;
  }

  inline float degToRad(float degrees) {
    return degrees * kDegToRad;
  }
}

#endif
