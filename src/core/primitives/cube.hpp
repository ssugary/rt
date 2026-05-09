#ifndef CUBE_HPP
#define CUBE_HPP
#include "square.hpp"

    namespace rt{
    class Cube : public Primitive {
    private:
        std::vector<std::shared_ptr<Square>> faces; 

    public:
        Cube(Point3 center, double size, std::shared_ptr<Material> mat);
        bool intersect(const Ray &r, Surfel *sf) const override;

        bool intersect_p(const Ray &r) const override;
    };
}
#endif