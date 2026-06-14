#ifndef BVH_ACCEL_HPP
#define BVH_ACCEL_HPP

#include "fbounds.hpp"
#include "primitive.hpp"
#include <algorithm>

namespace rt {

struct BVHNode {
  Bounds3f bounds;

  std::unique_ptr<BVHNode> left = nullptr;
  std::unique_ptr<BVHNode> right = nullptr;

  std::vector<std::shared_ptr<Primitive>> primitives;

  bool is_leaf() const { return left == nullptr && right == nullptr; }
};

class BVHAccel : public AggregatePrimitive {
private:
  std::unique_ptr<BVHNode> root = nullptr;
  int max_prims_per_node;

  std::unique_ptr<BVHNode> create_bvh(std::vector<std::shared_ptr<Primitive>> &prims);

  bool intersect_node(const BVHNode *node, const Ray &ray, Surfel *isect) const;

  bool intersect_p_node(const BVHNode *node, const Ray &ray) const;


public:
  BVHAccel(const std::vector<std::shared_ptr<Primitive>> &prims, int n_prims) {
    auto dummy = prims;
	max_prims_per_node = n_prims;
    root = create_bvh(dummy);
  }

  bool intersect(const Ray &ray, Surfel *isect) const;

  bool intersect_p(const Ray &ray) const;

  bool box(Bounds3f &box) const;

  void print();
};

} // namespace rt

#endif //< BVH_ACCEL_HPP

