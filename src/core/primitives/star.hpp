#ifndef STAR_HPP
#define STAR_HPP

#include "triangle.hpp"
#include <memory>

namespace rt{

    class Star : public Primitive{
        private:
            std::vector<std::shared_ptr<Triangle>> faces;
        public:
        Star(Point3 p, double rinn, double rout, std::shared_ptr<Material> mat);
        bool intersect(const Ray &r, Surfel *sf) const override;
        bool intersect_p(const Ray &r) const override;
    };
}

#endif