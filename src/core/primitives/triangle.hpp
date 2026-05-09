#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "primitive.hpp"
#include <memory>

namespace rt {

    class Triangle : public Primitive{
        private:
            Point3 p0;
            Point3 p1;
            Point3 p2;
        public:
            Triangle(Point3 p0, Point3 p1, Point3 p2, std::shared_ptr<Material> mat);
            bool intersect(const Ray &r, Surfel *sf) const override;
            bool intersect_p(const Ray &r) const override;
    };

};

#endif