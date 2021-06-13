#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include <math/utils.h>
#include <math/ray.h>
#include <math/vec3.h>

class camera {
  public:
    camera(
        point3 position,
        point3 focalPoint,
        vec3 cameraUp,
        double vFovDegrees,
        double aspectRatio,
        double aperture,
        double focusDist) {
      const double vFovRadians = degreesToRadians(vFovDegrees);
      const double h = tan(vFovRadians / 2);
      const double viewportHeight = 2.0 * h;
      const double viewportWidth = aspectRatio * viewportHeight;

      w = unitVector(position - focalPoint);
      u = unitVector(cross(cameraUp, w));
      vec3 v = cross(w, u);

      origin = position;
      horizontal = focusDist * viewportWidth * u;
      vertical = focusDist * viewportHeight * v;
      lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDist * w;

      lensRadius = aperture / 2;
    }

    ray getRay(double s, double t) const {
      const vec3 rd = lensRadius * randomInUnitDisk();
      const vec3 offset = u * rd.x() + v * rd.y();
      return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
    }

  private:
    point3 origin;
    point3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lensRadius;
};

#endif
