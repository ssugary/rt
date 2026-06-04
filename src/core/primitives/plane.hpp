#ifndef PLANE_HPP
#define PLANE_HPP

#include "primitive.hpp"
#include <memory>

namespace rt{
    class Plane : public Shape {
        private:
            Point3 p;
            Vec3 n;
        public:
        Plane(bool flip, Point3 p, Vec3 n): Shape(flip), p(p), n(n) {};
        bool intersect(const Ray &r, float* t_hit, Surfel *sf) const override;
        bool intersect_p(const Ray &r) const override;
        Bounds3f world_bounds() const override {};
    };
}

#endif