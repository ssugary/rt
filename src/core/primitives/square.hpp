#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "triangle.hpp"
#include <memory>

namespace rt {

    class Square : public Primitive{
        private:
            std::shared_ptr<Triangle> t1;
            std::shared_ptr<Triangle> t2;
        public:
            Square(Point3 p0, Point3 p1, Point3 p2, Point3 p3, std::shared_ptr<Material> mat);
            bool intersect(const Ray &r, Surfel *sf) const override;
            bool intersect_p(const Ray &r) const override;
    };

};

#endif