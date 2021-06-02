#include <iostream>

#include <math/constants.h>
#include <math/vec3.h>
#include <math/color.h>
#include <math/ray.h>
#include <graphics/hittableList.h>
#include <graphics/sphere.h>

color rayColor(const ray& r, const hittable& world) {
  hitRecord rec;
  if (world.hit(r, 0, INF, rec)) {
    return 0.5 * (rec.normal + color(1, 1, 1));
  }

  vec3 unitDirection = unitVector(r.direction());
  double t = 0.5 * (unitDirection.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  // Image
  const double aspectRatio = 16.0 / 9.0;
  const int imageWidth = 400;
  const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

  // World
  hittableList world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

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
      color pixelColor = rayColor(r, world);
      writeColor(std::cout, pixelColor);
    }
  }

  std::cerr << "\nDone.\n";
}
