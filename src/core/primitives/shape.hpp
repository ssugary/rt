#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"

#ifndef SURFEL_HPP
namespace rt {
class Shape;
}
#include "surfel.hpp"
#endif //< SURFEL_HPP

namespace rt {

class Shape {
protected:
  bool flips_normal;

public:
  Shape(bool flip) : flips_normal(flip) {};

  virtual Bounds3f world_bounds() const = 0;
  virtual bool intersect(const Ray &r, float *t_hit, Surfel *sf) const = 0;
  virtual bool intersect_p(const Ray &r) const = 0;
};

} // namespace rt
#endif //< SHAPE_HPP
