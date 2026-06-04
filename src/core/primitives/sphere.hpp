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
  Sphere(Point3 center, float radius, std::shared_ptr<Material> mat);

  bool intersect(const Ray &r, float *t_hit, Surfel *sf) const override;
  bool intersect_p(const Ray &r) const override;
};
} // namespace rt
#endif
