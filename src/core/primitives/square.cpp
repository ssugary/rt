#include "square.hpp"
#include "triangle.hpp"
#include <memory>

namespace rt{

    Square::Square(Point3 p0, Point3 p1, Point3 p2, Point3 p3, std::shared_ptr<Material> mat){
        this->t1 = std::make_shared<Triangle>(p0, p1, p2, mat);
        this->t2 = std::make_shared<Triangle>(p0, p2, p3, mat);
        this->material = mat;
    }

    bool Square::intersect(const Ray &r, Surfel *sf) const{
        if(t1->intersect(r, sf))return true;
        if(t2->intersect(r, sf))return true;
        return false;
    }
    bool Square::intersect_p(const Ray &r) const {
        if(t1->intersect_p(r))return true;
        if(t2->intersect_p(r))return true;
        return false;
    }
}