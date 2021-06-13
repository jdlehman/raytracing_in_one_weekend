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

hittableList randomScene() {
  hittableList world;
  auto groundMaterial = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, groundMaterial));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto chooseMat = randomDouble();
      point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        std::shared_ptr<material> sphereMaterial;

        if (chooseMat < 0.8) {
          // diffuse
          const color albedo = color::random() * color::random();
          sphereMaterial = std::make_shared<lambertian>(albedo);
        } else if (chooseMat < 0.95) {
          //metal
          const color albedo = color::random(0.5, 1);
          const double fuzz = randomDouble(0, 0.5);
          sphereMaterial = std::make_shared<metal>(albedo, fuzz);
        } else {
          // glass
          sphereMaterial = std::make_shared<dielectric>(1.5);
        }
        world.add(std::make_shared<sphere>(center, 0.2, sphereMaterial));
      }
    }
  }

  auto glassMaterial = std::make_shared<dielectric>(1.5);
  world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, glassMaterial));

  auto diffuseMaterial = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, diffuseMaterial));

  auto metalMaterial = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, metalMaterial));

  return world;
}

int main() {
  // Image
  const double aspectRatio = 3.0 / 2.0;
  const int imageWidth = 1200;
  const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
  const int samplesPerPixel = 100;
  const int maxRayBounces = 50;

  // World
  hittableList world = randomScene();

  // Camera
  const point3 position(13, 2, 3);
  const point3 focalPoint(0, 0, 0);
  const vec3 cameraUp(0, 1, 0);
  const double distToFocus = 10.0;
  const double aperture = 0.1;
  const double vFov = 20.0;
  camera cam(position, focalPoint, cameraUp, vFov, aspectRatio, aperture, distToFocus);

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
  return 0;
}
