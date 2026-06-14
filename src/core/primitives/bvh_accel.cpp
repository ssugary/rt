#include "bvh_accel.hpp"
#include "api.hpp"
#include <algorithm>
#include <memory>

namespace rt {

std::unique_ptr<BVHNode>
BVHAccel::create_bvh(std::vector<std::shared_ptr<Primitive>> &prims) {
  if (prims.empty())
    return nullptr;

  auto node = std::make_unique<BVHNode>();

  Bounds3f node_box;
  bool first = true;

  for (const auto &prim : prims) {
    Bounds3f b;

    // evita fazer box no plano
    if (!prim->box(b))
      continue;

    if (first) {
      node_box = b;
      first = false;
    } else {
      // cria o box da raiz
      node_box = node_box.merge(b);
    }
  }

  node->bounds = node_box;

  // caso base
  if (prims.size() <= max_prims_per_node) {
    node->primitives = prims;
    return node;
  }

  Bounds3f centroid_bounds;
  first = true;

  for (const auto &prim : prims) {

    Bounds3f b;
    prim->box(b);

    Point3 c = b.centroid();

    Bounds3f centroid_box(c, c);

    if (first) {
      centroid_bounds = centroid_box;
      first = false;
    } else {
      centroid_bounds = centroid_bounds.merge(centroid_box);
    }
  }

  Vec3 diag = centroid_bounds.max() - centroid_bounds.min();

  int axis;

  if (diag.x() > diag.y() && diag.x() > diag.z()) {
    axis = 0;
  } else if (diag.y() > diag.z()) {
    axis = 1;
  } else {
    axis = 2;
  }

  // ordena ao longo do deixo escolhido
  std::sort(prims.begin(), prims.end(), [axis](const auto &a, const auto &b) {
    Bounds3f ba;
    Bounds3f bb;

    a->box(ba);
    b->box(bb);

    return ba.centroid()[axis] < bb.centroid()[axis];
  });

  size_t mid = prims.size() / 2;

  // Avoid degenerate splits: if all centroids are identical or split is empty
  if (mid == 0 || mid == prims.size()) {
    node->primitives = prims;
    return node;
  }

  std::vector<std::shared_ptr<Primitive>> leftPrims(prims.begin(),
                                                    prims.begin() + mid);

  std::vector<std::shared_ptr<Primitive>> rightPrims(prims.begin() + mid,
                                                     prims.end());
  // cria os nós
  node->left = create_bvh(leftPrims);
  node->right = create_bvh(rightPrims);

  return node;
}

bool BVHAccel::intersect_node(const BVHNode *node, const Ray &ray,
                              Surfel *isect) const {

  if (!node)
    return false;

  float tmin = 0.f;
  float tmax = std::numeric_limits<float>::max();

  if (!node->bounds.intersect_p(ray, tmin, tmax)) {
    return false;
  }

  if (node->is_leaf()) {

    bool hit = false;

    float closest_t = std::numeric_limits<float>::max();

    Surfel closest_sf;

    for (const auto &prim : node->primitives) {
      Surfel temp;

      if (prim->intersect(ray, &temp)) {
        if (temp.time < closest_t) {
          closest_t = temp.time;

          closest_sf = temp;

          hit = true;
        }
      }
    }

    if (hit && isect)
      *isect = closest_sf;

    return hit;
  }

  Surfel left_sf;
  Surfel right_sf;

  bool hit_left = intersect_node(node->left.get(), ray, &left_sf);

  bool hit_right = intersect_node(node->right.get(), ray, &right_sf);

  if (!hit_left && !hit_right) {
    return false;
  }

  if (hit_left && !hit_right) {
    if (isect)
      *isect = left_sf;

    return true;
  }

  if (!hit_left && hit_right) {
    if (isect)
      *isect = right_sf;

    return true;
  }

  if (left_sf.time < right_sf.time) {
    if (isect)
      *isect = left_sf;
  } else {
    if (isect)
      *isect = right_sf;
  }

  return true;
}

bool BVHAccel::intersect_p_node(const BVHNode *node, const Ray &ray) const {

  if (!node)
    return false;

  float tmin = 0.f;
  float tmax = std::numeric_limits<float>::max();

  if (!node->bounds.intersect_p(ray, tmin, tmax)) {
    return false;
  }

  if (node->is_leaf()) {

    for (const auto &prim : node->primitives) {
      if (prim->intersect_p(ray))
        return true;
    }

    return false;
  }

  return intersect_p_node(node->left.get(), ray) ||
         intersect_p_node(node->right.get(), ray);
}

bool BVHAccel::intersect(const Ray &ray, Surfel *isect) const {
  if (!root)
    return false;

  return intersect_node(root.get(), ray, isect);
}

bool BVHAccel::intersect_p(const Ray &ray) const {
  if (!root)
    return false;

  return intersect_p_node(root.get(), ray);
}

namespace {
void print_bvh_node(const rt::BVHNode *node, int depth = 0) {
    if (!node) return;
    std::string indent(depth * 2, ' ');
    const auto& b = node->bounds;
    std::cout << indent << (node->is_leaf() ? "Leaf Node" : "Interior Node") << ":\n";
    std::cout << indent << "  pmin: " << b.min() << " | pmax: " << b.max() << "\n";
    std::cout << indent << "  centroid: " << b.centroid() << "\n";
    if (node->is_leaf()) {
        std::cout << indent << "  Primitives: " << node->primitives.size() << std::endl;
    }
    if (!node->is_leaf()) {
        print_bvh_node(node->left.get(), depth + 1);
        print_bvh_node(node->right.get(), depth + 1);
    }
}
}

void BVHAccel::print() {
    print_bvh_node(root.get());
}

bool BVHAccel::box(Bounds3f &box) const {
  if (root == nullptr)
    return false;

  box = root->bounds;
  return true;
}

} // namespace rt
