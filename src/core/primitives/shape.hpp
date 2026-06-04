#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"
#include "surfel.hpp"

namespace rt {

class Shape {
private:
  bool flips_normal;

public:
  Shape(bool flip) : flips_normal(flip) {};

  virtual bool intersect(const Ray &r, float *t_hit, Surfel *sf) const = 0;
  virtual bool intersect_p(const Ray &r) const = 0;
};

} // namespace rt
#endif //< SHAPE_HPP
