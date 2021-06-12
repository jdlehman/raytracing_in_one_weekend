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
    metal(const color& a) : albedo(a) {}

    virtual bool scatter(const ray &rayIn, const hitRecord& hitRec, color& attenuation, ray& scattered) const override {
      vec3 reflected = reflect(unitVector(rayIn.direction()), hitRec.normal);
      scattered = ray(hitRec.p, reflected);
      attenuation = albedo;
      return dot(scattered.direction(), hitRec.normal) > 0;
    }

  public:
    color albedo;
};


#endif
