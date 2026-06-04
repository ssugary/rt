#include "primitive.hpp"
#include "error.hpp"

namespace rt{

const std::shared_ptr<Material> AggregatePrimitive::get_material() const {
	WARNING("AggregatePrimitive must not have a material");
	return nullptr;
}

void PrimitiveList::add(std::shared_ptr<Primitive> object) {
    primitives.push_back(object);
}

bool PrimitiveList::intersect(const Ray& ray, Surfel* isect) const {
    Surfel temp_isect;
    bool hit_anything = false;
    for (const auto& object : primitives) {
        if (object->intersect(ray, &temp_isect)) {
            hit_anything = true;
            if (isect) {
                *isect = temp_isect;
            }
        }
    }
    return hit_anything;
}

bool PrimitiveList::intersect_p(const Ray& ray) const  {
    for (const auto& object : primitives) {
        if (object->intersect_p(ray)) {
            return true;
        }
    }
    return false;
}}
