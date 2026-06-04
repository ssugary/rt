#ifndef SPHERE_PRIMITIVE_HPP
#define SPHERE_PRIMITIVE_HPP

#include "primitive.hpp"
#include "shape.hpp"

namespace rt {
class Sphere : public Shape {
private:
  Point3 center{};
  double radius{0};

public:
  Sphere(bool flip, Point3 center, float radius)
        : Shape(flip), center(center), radius(radius) {};

  bool intersect(const Ray &r, float *t_hit, Surfel *sf) const override;
  bool intersect_p(const Ray &r) const override;
  Bounds3f world_bounds() const override {};
};
} // namespace rt
#endif
