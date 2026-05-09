#ifndef SPHERE_PRIMITIVE_HPP
#define SPHERE_PRIMITIVE_HPP

#include "primitive.hpp"

namespace rt{
class Sphere : public Primitive {
    private:
        Point3 center{};
        double radius{0};

    public:
        Sphere(Point3 center, float radius,std::shared_ptr<Material> mat);
        bool intersect(const Ray &r, Surfel *sf) const override;
        bool intersect_p(const Ray &r) const override;
};
}
#endif