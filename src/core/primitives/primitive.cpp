#include "primitive.hpp"
#include "error.hpp"

namespace rt {

const std::shared_ptr<Material> AggregatePrimitive::get_material() const {
  WARNING("AggregatePrimitive must not have a material");
  return nullptr;
}

void PrimitiveList::add(const std::shared_ptr<Primitive> &object) {
  primitives.push_back(object);
}


std::unique_ptr<BVHNode> create_bvh(std::vector<std::shared_ptr<Primitive>> &prims) {
  if (prims.empty())
    return nullptr;

  auto node = std::make_unique<BVHNode>();

  //--------------------------------------------------
  // Calcula bounding box do nó
  //--------------------------------------------------

  Bounds3f node_box;
  bool first = true;

  for (const auto &prim : prims) {

    Bounds3f b;

    if (!prim->box(b))
      continue;

    if (first) {
      node_box = b;
      first = false;
    } else {
      node_box = node_box.merge(b);
    }
  }

  node->bounds = node_box;

  // caso base
  if (prims.size() <= 2) {
    node->primitives = prims;
    return node;
  }

  //--------------------------------------------------
  // Bounding box dos centroides
  //--------------------------------------------------

  Bounds3f centroidBounds;
  first = true;

  for (const auto &prim : prims) {

    Bounds3f b;
    prim->box(b);

    Point3 c = b.centroid();

    Bounds3f centroidBox(c, c);

    if (first) {
      centroidBounds = centroidBox;
      first = false;
    } else {
      centroidBounds = centroidBounds.merge(centroidBox);
    }
  }

  //--------------------------------------------------
  // Escolhe eixo de maior extensão
  //--------------------------------------------------

  Vec3 diag = centroidBounds.max() - centroidBounds.min();

  int axis;

  if (diag.x() > diag.y() && diag.x() > diag.z()) {
    axis = 0;
  } else if (diag.y() > diag.z()) {
    axis = 1;
  } else {
    axis = 2;
  }

  //--------------------------------------------------
  // Ordena pelos centroides
  //--------------------------------------------------

  std::sort(prims.begin(), prims.end(), [axis](const auto &a, const auto &b) {
    Bounds3f ba;
    Bounds3f bb;

    a->box(ba);
    b->box(bb);

    return ba.centroid()[axis] < bb.centroid()[axis];
  });

  //--------------------------------------------------
  // Divide ao meio
  //--------------------------------------------------

  size_t mid = prims.size() / 2;

  std::vector<std::shared_ptr<Primitive>> leftPrims(prims.begin(),
                                                    prims.begin() + mid);

  std::vector<std::shared_ptr<Primitive>> rightPrims(prims.begin() + mid,
                                                     prims.end());

  //--------------------------------------------------
  // Recursão
  //--------------------------------------------------

  node->left = create_bvh(leftPrims);
  node->right = create_bvh(rightPrims);

  return node;
}

bool PrimitiveList::intersect(const Ray &ray, Surfel *isect) const {
  Surfel temp_isect;
  bool hit_anything = false;
  for (const auto &object : primitives) {
    if (object->intersect(ray, &temp_isect)) {
      hit_anything = true;
      if (isect) {
        *isect = temp_isect;
      }
    }
  }
  return hit_anything;
}

bool PrimitiveList::intersect_p(const Ray &ray) const {
  for (const auto &object : primitives) {
    if (object->intersect_p(ray)) {
      return true;
    }
  }
  return false;
}

bool GeometricPrimitive::intersect(const Ray &r, Surfel *sf) const {
  float t_hit;
  if (shape->intersect(r, &t_hit, sf)) {
    r.setTMax(t_hit);
    if (sf) {
      sf->primitive = this;
    }
    return true;
  }
  return false;
}

bool GeometricPrimitive::intersect_p(const Ray &r) const {
  return shape->intersect_p(r);
}

Bounds3f GeometricPrimitive::box() const { return shape->box(); }

} // namespace rt
