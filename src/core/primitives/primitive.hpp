#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <memory>

#include "material.hpp"
#include "ray.hpp"

#ifndef SURFEL_HPP
namespace rt{
class Primitive;
}
#include "surfel.hpp"
#endif  //< SURFEL_HPP

namespace rt{
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
}
#endif //< PRIMITIVE_HPP

#ifndef PRIMITIVE_LIST_HPP
#define PRIMITIVE_LIST_HPP

namespace rt{
    
class PrimitiveList : public Primitive {
  private:
    std::vector<std::shared_ptr<Primitive>> primitives;
  public:
    void add(std::shared_ptr<Primitive> primitive);
    bool intersect(const Ray& ray, Surfel* isect) const override;
    bool intersect_p(const Ray &ray) const override;
    
};
}

#endif //< PRIMITIVE_LIST_HPP