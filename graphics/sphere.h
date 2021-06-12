#ifndef SPHERE_H
#define SPHERE_H
#pragma once

#include "hittable.h"
#include <math/vec3.h>

class sphere : public hittable {
  public:
    sphere() {}
    virtual ~sphere() {}
    sphere(point3 cen, double r, std::shared_ptr<material> m) : center(cen), radius(r), mat(m) {};

    virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& hitRec) const override;

  public:
    point3 center;
    double radius;
    std::shared_ptr<material> mat;
};

bool sphere::hit(const ray& r, double tMin, double tMax, hitRecord& hitRec) const  {
  // x^2  + y^2 + z^2 = r^2
  // dot(point - center, point - center) = r^2
  // solve with quadratic equation:
  // positive means 2 solitions, 0 means 1 solution, negativ means no real solutions
  const vec3 oc = r.origin() - center;
  const double a = r.direction().lengthSquared();
  const double halfB = dot(oc, r.direction());
  const double c = oc.lengthSquared() - radius * radius;

  const double discriminant = halfB * halfB - a * c;
  if (discriminant < 0) return false;
  const double sqrtd = sqrt(discriminant);

  // find the nearest root that lies in the acceptable range
  double root = (-halfB - sqrtd) / a;
  if (root < tMin || root > tMax) {
    // try the other root
    root = (-halfB + sqrtd) / a;
    if (root < tMin || root > tMax) {
      return false;
    }
  }

  hitRec.t = root;
  hitRec.p = r.at(hitRec.t);
  hitRec.normal = (hitRec.p - center) / radius;
  vec3 outwardNormal = (hitRec.p - center) / radius;
  hitRec.setFaceNormal(r, outwardNormal);
  hitRec.mat = mat;
  return true;
}

#endif
