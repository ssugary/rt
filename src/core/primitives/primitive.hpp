#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <memory>

#include "material.hpp"
#include "ray.hpp"

#ifndef SURFEL_HPP
namespace rt {
class Primitive;
}
#include "surfel.hpp"
#endif //< SURFEL_HPP

namespace rt {
constexpr float epsilon = std::numeric_limits<float>::epsilon();
class Primitive {
protected:
  std::shared_ptr<Material> material;

public:
  virtual ~Primitive() = default;
  virtual bool intersect(const Ray &r, Surfel *sf) const = 0;
  virtual bool intersect_p(const Ray &r) const = 0;
  virtual const std::shared_ptr<Material> get_material() const {
    return material;
  }
};
} // namespace rt
#endif //< PRIMITIVE_HPP

#ifndef AGGREGATE_PRIMITIVE_HPP
#define AGGREGATE_PRIMITIVE_HPP

#include <memory>

namespace rt {

class AggregatePrimitive : public Primitive {
public:
  const std::shared_ptr<Material> get_material() const override;
};

} // namespace rt
#endif //< AGGREGATE_PRIMITIVE_HPP

#ifndef PRIMITIVE_LIST_HPP
#define PRIMITIVE_LIST_HPP

namespace rt {

class PrimitiveList : public AggregatePrimitive {
private:
  std::vector<std::shared_ptr<Primitive>> primitives;

public:
  void add(std::shared_ptr<Primitive> primitive);
  bool intersect(const Ray &ray, Surfel *isect) const override;
  bool intersect_p(const Ray &ray) const override;
};
} // namespace rt

#endif //< PRIMITIVE_LIST_HPP

#ifndef GEOMETRIC_PRIMITIVE_HPP
#define GEOMETRIC_PRIMITIVE_HPP

#include "shape.hpp"
#include <memory>

namespace rt {

class GeometricPrimitive : public Primitive {
private:
  std::shared_ptr<Shape> shape;
  std::shared_ptr<Material> material;

  GeometricPrimitive(std::shared_ptr<Shape> shape,
                     std::shared_ptr<Material> material)
      : shape(shape), material(material) {};

  void set_material(std::shared_ptr<Material> m) { material = m; };
};
} // namespace rt
#endif //< GEOMETRIC_PRIMITIVE_HPP
