#ifndef PRIMITIVE_HPP
namespace rt {
class Bounds3f;
}
#include "primitive.hpp"
#endif

#ifndef FLOAT_BOUND_HPP
#define FLOAT_BOUND_HPP

#include "common.hpp"
#include "ray.hpp"
#include <limits>
#include <memory>

namespace rt {

inline float ffmin(const float &a, const float &b) { return a < b ? a : b; }
inline float ffmax(const float &a, const float &b) { return a > b ? a : b; }

class Bounds3f {
private:
  Point3 pmin;
  Point3 pmax;

public:
  Bounds3f() {
    float inf = std::numeric_limits<float>::infinity();

    pmin = Point3(inf, inf, inf);
    pmax = Point3(-inf, -inf, -inf);
  }
  Bounds3f(const Point3 &p1, const Point3 &p2) {
    pmin = {std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
            std::min(p1.z(), p2.z())};
    pmax = {std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()),
            std::max(p1.z(), p2.z())};
  };

  Bounds3f(std::vector<std::shared_ptr<Primitive>> &prims) {
    std::numeric_limits<float>::infinity();

    Point3 p_min = Point3(std::numeric_limits<float>::infinity(),
                          std::numeric_limits<float>::infinity(),
                          std::numeric_limits<float>::infinity());
    Point3 p_max = Point3(-std::numeric_limits<float>::infinity(),
                          -std::numeric_limits<float>::infinity(),
                          -std::numeric_limits<float>::infinity());
    for (auto p : prims) {
      Bounds3f b;
      if (p->box(b)) {
        p_min = Point3(std::min(p_min.x(), b.pmin.x()),
                       std::min(p_min.y(), b.pmin.y()),
                       std::min(p_min.z(), b.pmin.z()));
        p_max = Point3(std::max(p_max.x(), b.pmax.x()),
                       std::max(p_max.y(), b.pmax.y()),
                       std::max(p_max.z(), b.pmax.z()));
      }
    }
    pmin = p_min;
    pmax = p_max;
  };

  // Getters
  const Point3 &min() const { return pmin; }
  const Point3 &max() const { return pmax; }

  void print() {
    cout << "pmin: " << pmin << " | " << "pmax: " << pmax << "\n";
  }

  Point3 centroid() const { return (pmin + pmax) * 0.5; }

  Bounds3f merge(const Bounds3f &b) const {
    return Bounds3f(
        Point3(std::min(pmin.x(), b.pmin.x()), std::min(pmin.y(), b.pmin.y()),
               std::min(pmin.z(), b.pmin.z())),
        Point3(std::max(pmax.x(), b.pmax.x()), std::max(pmax.y(), b.pmax.y()),
               std::max(pmax.z(), b.pmax.z())));
  }

  bool intersect_p(const Ray &ray, float &tmin, float &tmax) const {
    for (int a = 0; a < 3; ++a) {
      float invD = 1.0f / ray.getDirection()[a];
      float t0 = (pmin[a] - ray.getOrigin()[a]) * invD;
      float t1 = (pmax[a] - ray.getOrigin()[a]) * invD;
      if (invD < 0.0f) std::swap(t0, t1);
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
