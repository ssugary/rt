#ifndef FLOAT_BOUND_HPP
#define FLOAT_BOUND_HPP

#include "common.hpp"
#include "primitive.hpp"
#include "ray.hpp"
#include <memory>

namespace rt {

inline float ffmin(const float &a, const float &b) { return a < b ? a : b; }
inline float ffmax(const float &a, const float &b) { return a > b ? a : b; }

class Bounds3f {
private:
  Point3 pmin;
  Point3 pmax;

public:
  Bounds3f();
  Bounds3f(const Point3 &p1, const Point3 &p2) {
    pmin = {std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
            std::min(p1.z(), p2.z())};
    pmax = {std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()),
            std::max(p1.z(), p2.z())};
  };

  Bounds3f(std::vector<std::shared_ptr<Primitive>> &prims) {
    Point3 p_min;
    Point3 p_max;
    // for (const auto p : prims) {
    //   Bounds3f b;
    //   if (p->box(b)) {
    //     if (b.pmin < p_min) {
    //     }
    //   }
    // }
  };

  // Getters
  const Point3 &min() const { return pmin; }
  const Point3 &max() const { return pmax; }

  Point3 centroid() const { return (pmin + pmax) * 0.5; }

  Bounds3f merge(const Bounds3f &b) const {
    return Bounds3f(
        Point3(std::min(pmin.x(), b.pmin.x()), std::min(pmin.y(), b.pmin.y()),
               std::min(pmin.z(), b.pmin.z())),
        Point3(std::max(pmax.x(), b.pmax.x()), std::max(pmax.y(), b.pmax.y()),
               std::max(pmax.z(), b.pmax.z())));
  }

  bool intersect_p(const Ray &ray, float &tmin, float &tmax) const {
    for (int a{0}; a < 3; ++a) {
      float t0 =
          ffmin((pmin[a] - ray.getOrigin()[a]) / ray.getDirection()[a],
                (pmax[a] - ray.getDirection()[a] / ray.getDirection()[a]));
      float t1 =
          ffmax((pmin[a] - ray.getOrigin()[a]) / ray.getDirection()[a],
                (pmax[a] - ray.getDirection()[a] / ray.getDirection()[a]));
      tmin = ffmax(t0, tmin);
      tmax = ffmin(t1, tmax);

      if (tmax <= tmin)
        return false;
    }
    return true;
  }
};

}; // namespace rt

#endif
