#include <iostream>

#include <math/vec3.h>
#include <math/color.h>
#include <math/ray.h>

double hitSphere(const point3& center, double radius, const ray& r) {
  // x^2  + y^2 + z^2 = r^2
  // dot(point - center, point - center) = r^2
  // solve with quadratic equation:
  // positive means 2 solitions, 0 means 1 solution, negativ means no real solutions
  const vec3 oc = r.origin() - center;
  const double a = dot(r.direction(), r.direction());
  const double b = 2.0 * dot(oc, r.direction());
  const double c = dot(oc, oc) - radius * radius;
  const double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return -1.0;
  } else {
    // - instead of +- beause we want the closest hit point (smallest t)
    return (-b - sqrt(discriminant)) / 2.0 * a;
  }
}

color rayColor(const ray& r) {
  double t = hitSphere(point3(0, 0, -1), 0.5, r);
  if (t > 0.0) {
    vec3 normal = unitVector(r.at(t) - vec3(0, 0, -1));
    return 0.5 * color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
  }
  vec3 unitDirection = unitVector(r.direction());
  t = 0.5 * (unitDirection.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  // Image
  const double aspectRatio = 16.0 / 9.0;
  const int imageWidth = 400;
  const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

  // Camera
  const double viewportHeight = 2.0;
  const double viewportWidth = aspectRatio * viewportHeight;
  const double focalLength = 1.0;

  const point3 origin = point3(0, 0, 0);
  const vec3 horizontal = vec3(viewportWidth, 0, 0);
  const vec3 vertical = vec3(0, viewportHeight, 0);
  const vec3 lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

  // Render
  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int i = imageHeight - 1; i >= 0; --i) {
    std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
    for (int j = 0; j < imageWidth; ++j) {
      double u = double(j) / (imageWidth - 1);
      double v = double(i) / (imageHeight - 1);
      ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
      color pixelColor = rayColor(r);
      writeColor(std::cout, pixelColor);
    }
  }

  std::cerr << "\nDone.\n";
}
