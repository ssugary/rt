#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "primitive.hpp"
#include <memory>

namespace rt {

    class Triangle : public Shape{
        private:
            Point3 p0;
            Point3 p1;
            Point3 p2;
        public:
            Triangle(bool flip, Point3 p0, Point3 p1, Point3 p2);
            bool intersect(const Ray &r, float *t_hit, Surfel *sf) const override;
            bool intersect_p(const Ray &r) const override;
            Bounds3f world_bounds() const override {};
    };

};

#endif