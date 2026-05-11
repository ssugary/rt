#include "star.hpp"
#include "surfel.hpp"
#include <cmath>

namespace rt{

    Star::Star(Point3 p, double rinn, double rout, std::shared_ptr<Material> mat){
        this->material = mat;

        std::vector<Point3> edgePoints;
        
        for(int i{0}; i < 10; ++i){
            double r = rinn;
            if(i % 2 == 0){
                r = rout;
            }
            double theta = M_PI/4.0; //M_PI/2.0;
            theta += i * (M_PI / 5.0);

            double x = p[0] + (r * std::cos(theta));
            double y = p[1] + (r * std::sin(theta));
            double z = p[2];

            edgePoints.push_back({x, y, z});
        }

        for (int i = 0; i < 10; ++i) {
            Point3 p1 = edgePoints[i];
            Point3 p2 = edgePoints[(i + 1) % 10];
            
            faces.push_back(std::make_shared<Triangle>(p, p1, p2, mat));
        }

    }

    bool Star::intersect(const Ray &r, Surfel *sf) const {
        bool hit{false};
        for(const auto& face : faces){
            if(face->intersect(r, sf)){
                hit = true;
            }
        }
        return hit;
    }

    bool Star::intersect_p(const Ray &r) const {
        for(const auto& face : faces){
            if(face->intersect_p(r))return true;
        }
        return false;
    }


}