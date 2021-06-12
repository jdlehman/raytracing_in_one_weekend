#include <iostream>

#include <math/ray.h>
#include <math/utils.h>
#include <math/vec3.h>
#include <graphics/camera.h>
#include <graphics/color.h>
#include <graphics/hittableList.h>
#include <graphics/sphere.h>
#include <graphics/material.h>

color rayColor(const ray& r, const hittable& world, int remainingRayBounces) {
  hitRecord rec;
  // no more light is gathered once passing the ray bounce limit
  if (remainingRayBounces <= 0) {
    return color(0, 0, 0);
  }
  if (world.hit(r, 0.001, INF, rec)) {
    ray scattered;
    color attenuation;
    if (rec.mat->scatter(r, rec, attenuation, scattered)) {
      return attenuation * rayColor(scattered, world, remainingRayBounces - 1);
    }
    return color(0, 0, 0);
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
  std::shared_ptr<lambertian> materialGround = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
  std::shared_ptr<lambertian> materialCenter = std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
  std::shared_ptr<metal> materialLeft = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
  std::shared_ptr<metal> materialRight = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

  world.add(std::make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, materialGround));
  world.add(std::make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, materialCenter));
  world.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
  world.add(std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, materialRight));

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
