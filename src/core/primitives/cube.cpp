#include "cube.hpp"

namespace rt{

    Cube::Cube(Point3 center, double size, std::shared_ptr<Material> mat) {
            this->material = mat;

            double h = size / 2.0;

            Point3 v0(center.x() - h, center.y() - h, center.z() - h);
            Point3 v1(center.x() + h, center.y() - h, center.z() - h);
            Point3 v2(center.x() + h, center.y() + h, center.z() - h);
            Point3 v3(center.x() - h, center.y() + h, center.z() - h);
            Point3 v4(center.x() - h, center.y() - h, center.z() + h);
            Point3 v5(center.x() + h, center.y() - h, center.z() + h);
            Point3 v6(center.x() + h, center.y() + h, center.z() + h);
            Point3 v7(center.x() - h, center.y() + h, center.z() + h);

            faces.push_back(std::make_shared<Square>(v4, v5, v6, v7, mat)); 
            faces.push_back(std::make_shared<Square>(v1, v0, v3, v2, mat)); 
            faces.push_back(std::make_shared<Square>(v3, v2, v6, v7, mat));
            faces.push_back(std::make_shared<Square>(v4, v5, v1, v0, mat)); 
            faces.push_back(std::make_shared<Square>(v5, v1, v2, v6, mat));
            faces.push_back(std::make_shared<Square>(v0, v4, v7, v3, mat));
        }

    bool Cube::intersect(const Ray &r, Surfel *sf) const {
        bool hit{false};
            for (const auto& face : faces) {
                if (face->intersect(r, sf)) {
                    hit = true;
                }
            }
            return hit;
        }

    bool Cube::intersect_p(const Ray &r) const {
            for (const auto& face : faces) {
                if (face->intersect_p(r)) return true;
            }
            return false;
        }


}