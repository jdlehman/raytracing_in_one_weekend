#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

const double INF = std::numeric_limits<double>::infinity();

inline double degreesToRadians(double degrees) {
  return degrees * M_PI / 180.0;
}

inline double randomDouble() {
  // returns a random real in [0, 1)
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double randomDouble(double min, double max) {
  // returns a random real in [min, max)
  return min + (max - min) * randomDouble();
}

inline double clamp(double x, double min, double max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

#endif
