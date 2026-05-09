
#include "plane.hpp"
#include <cstdlib>

namespace rt{

        Plane::Plane(Point3 p, Vec3 n, std::shared_ptr<Material> mat)
            : p(p), n(n) {
            this->material = mat;
        }

        bool Plane::intersect(const Ray &r, Surfel *sf) const {
             double dn = dot(r.getDirection(), n);

            if(std::abs(dn) < epsilon){
                return false;
            }
            Point3 p = this->p - r.getOrigin();
            double t = dot(p, n);

            if(t == 0){
                return false;
            }
            t /= dn;
            if(t < r.getTMin() || t > r.getTMax()){
                return false;
            }

            if(sf){
                sf->time = t;
                sf->n = this->n;
                
                if(dn > 0)sf->n = -this->n;

                sf->p = r(t);
                sf->wo = -r.getDirection();
                sf->primitive = this;
            }
            r.setTMax(t);

            return true;
        }
        bool Plane::intersect_p(const Ray &r) const {
        
            double dn = dot(r.getDirection(), n);

            if(std::abs(dn) < epsilon){
                return false;
            }
            Point3 p = this->p - r.getOrigin();
            double t = dot(p, n);

            if(t == 0){
                return false;
            }
            t /= dn;
            if(t < r.getTMin() || t > r.getTMax()){
                return false;
            }

            return true;
        }


}