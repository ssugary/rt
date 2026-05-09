#ifndef PLANE_HPP
#define PLANE_HPP

#include "primitive.hpp"
#include <memory>

namespace rt{
    class Plane : public Primitive{
        private:
            Point3 p;
            Vec3 n;
        public:
        Plane(Point3 p, Vec3 n, std::shared_ptr<Material> mat);
        bool intersect(const Ray &r, Surfel *sf) const override;
        bool intersect_p(const Ray &r) const override;
    };
}

#endif