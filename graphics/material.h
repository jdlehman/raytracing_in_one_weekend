#ifndef MATERIAL_H
#define MATERIAL_H
#pragma once

#include <math/ray.h>
#include <math/vec3.h>

struct hitRecord;

class material {
  public:
    virtual bool scatter(const ray& rayIn, const hitRecord& hitRec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
  public:
    virtual ~lambertian() {}
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(const ray& rayIn, const hitRecord& hitRec, color& attenuation, ray& scattered) const override {
      vec3 scatterDirection = hitRec.normal + randomUnitVector();

      // catch degenerate scatter direction
      // if unit vector is oposite of normal, the sum will be zero
      // which will cause bad situations (infinities and NaNs) downstream
      if (scatterDirection.nearZero()) {
        scatterDirection = hitRec.normal;
      }
      scattered = ray(hitRec.p, scatterDirection);
      attenuation = albedo;
      return true;
    }

  public:
    color albedo;
};

class metal : public material {
  public:
    virtual ~metal() {}
    metal(const color& a, double f) : albedo(a), fuzz(f < 1.0 ? f : 1.0) {}

    virtual bool scatter(const ray &rayIn, const hitRecord& hitRec, color& attenuation, ray& scattered) const override {
      vec3 reflected = reflect(unitVector(rayIn.direction()), hitRec.normal);
      scattered = ray(hitRec.p, reflected + fuzz * randomInUnitSphere());
      attenuation = albedo;
      return dot(scattered.direction(), hitRec.normal) > 0;
    }

  public:
    color albedo;
    double fuzz;
};

class dielectric : public material {
  public:
    virtual ~dielectric() {}
    dielectric(double indexOfRefraction) : indexOfRefraction(indexOfRefraction) {}

    virtual bool scatter(const ray& rayIn, const hitRecord& hitRec, color& attenuation, ray& scattered) const override {
      attenuation = color(1.0, 1.0, 1.0);
      double refractionRatio = hitRec.frontFace ? (1.0 / indexOfRefraction) : indexOfRefraction;

      vec3 unitDirection = unitVector(rayIn.direction());
      double cosTheta = fmin(dot(-unitDirection, hitRec.normal), 1.0);
      double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

      bool cannotRefract = refractionRatio * sinTheta > 1.0;
      vec3 direction;
      if (cannotRefract) {
        direction = reflect(unitDirection, hitRec.normal);
      } else {
        direction = refract(unitDirection, hitRec.normal, refractionRatio);
      }

      scattered = ray(hitRec.p, direction);
      return true;
    }

  public:
    double indexOfRefraction;
};


#endif
