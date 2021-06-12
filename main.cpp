#include <iostream>

#include <math/utils.h>
#include <math/vec3.h>
#include <math/color.h>
#include <math/ray.h>
#include <graphics/camera.h>
#include <graphics/hittableList.h>
#include <graphics/sphere.h>

color rayColor(const ray& r, const hittable& world, int remainingRayBounces) {
  hitRecord rec;
  // no more light is gathered once passing the ray bounce limit
  if (remainingRayBounces <= 0) {
    return color(0, 0, 0);
  }
  if (world.hit(r, 0.001, INF, rec)) {
    point3 target = rec.p + rec.normal + randomInUnitSphere();
    return 0.5 * rayColor(ray(rec.p, target - rec.p), world, remainingRayBounces - 1);
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
  const int samplesPerPixel = 100;
  const int maxRayBounces = 50;

  // World
  hittableList world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

  // Camera
  camera cam;

  // Render
  std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int i = imageHeight - 1; i >= 0; --i) {
    std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
    for (int j = 0; j < imageWidth; ++j) {
      color pixelColor(0, 0, 0);
      for (int s = 0; s < samplesPerPixel; ++s) {
        const double u = double(j + randomDouble()) / (imageWidth - 1);
        const double v = double(i + randomDouble()) / (imageHeight - 1);
        const ray r = cam.getRay(u, v);
        pixelColor += rayColor(r, world, maxRayBounces);
      }
      writeColor(std::cout, pixelColor, samplesPerPixel);
    }
  }

  std::cerr << "\nDone.\n";
}
