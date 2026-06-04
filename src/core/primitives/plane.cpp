
#include "plane.hpp"
#include "shape.hpp"
#include <cstdlib>

namespace rt{


        bool Plane::intersect(const Ray &r, float* t_hit, Surfel *sf) const {
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

            if(t_hit) *t_hit = t;


            if(sf){
                sf->time = t;
                sf->n = this->n;
                
                if(flips_normal || dn > 0) sf->n = -this->n;

                sf->p = r(t);
                sf->wo = -r.getDirection();
            }

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