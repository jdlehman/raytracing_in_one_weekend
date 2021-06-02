#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittableList : public hittable {
  public:
    hittableList() {}
    virtual ~hittableList() {}
    hittableList(std::shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& hitRec) const override;

  public:
    std::vector<std::shared_ptr<hittable>> objects;
};

bool hittableList::hit(const ray& r, double tMin, double tMax, hitRecord& hitRec) const {
  hitRecord tempRec;
  bool hitAnything = false;
  double closestSoFar = tMax;

  for (const auto& object : objects) {
    if (object->hit(r, tMin, closestSoFar, tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.t;
      hitRec = tempRec;
    }
  }

  return hitAnything;
}

#endif
