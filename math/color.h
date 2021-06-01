#ifndef COLOR_H
#define COLOR_H
#pragma once

#include "vec3.h"

#include <iostream>

void writeColor(std::ostream &out, color pixelColor) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixelColor.r()) << ' '
        << static_cast<int>(255.999 * pixelColor.g()) << ' '
        << static_cast<int>(255.999 * pixelColor.b()) << '\n';
}

#endif
