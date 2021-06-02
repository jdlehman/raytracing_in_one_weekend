#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <limits>
#include <memory>

const double INF = std::numeric_limits<double>::infinity();

inline double degreesToRadians(double degrees) {
  return degrees * M_PI / 180.0;
}

#endif
