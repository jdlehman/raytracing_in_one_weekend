#ifndef COLOR_H
#define COLOR_H
#pragma once

#include <math/vec3.h>
#include <math/utils.h>

#include <iostream>

void writeColor(std::ostream &out, color pixelColor, int samplesPerPixel) {
  double r = pixelColor.r();
  double g = pixelColor.g();
  double b = pixelColor.b();

  // divide color by the number of samples and gamma-correct for gamma=2.0
  const double scale = 1.0 / samplesPerPixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif
