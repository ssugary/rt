#ifndef FLOAT_BOUND_HPP
#define FLOAT_BOUND_HPP

#include "common.hpp"
#include "ray.hpp"

namespace rt {

    inline float ffmin(float a, float b) {return a < b ? a : b;}
    inline float ffmax(float a, float b) {return a > b ? a : b;}

    class Bounds3f{
        private:
            Point3 pmin;
            Point3 pmax;
        public:

        Bounds3f(const Point3& pmin, const Point3& pmax) 
        : pmin(pmin), pmax(pmax) {};

        bool intersect_p(const Ray& ray, float& tmin, float& tmax) const {
            for(int a{0}; a < 3; ++a){
                float t0 = ffmin((pmin[a] - ray.getOrigin()[a]) / ray.getDirection()[a],
                                 (pmax[a] -ray.getDirection()[a] / ray.getDirection()[a]));
                float t1 = ffmax((pmin[a] - ray.getOrigin()[a]) / ray.getDirection()[a],
                                 (pmax[a] -ray.getDirection()[a] / ray.getDirection()[a]));
                tmin = ffmax(t0, tmin);
                tmax = ffmin(t1, tmax);
                
                if(tmax <= tmin)return false;
            }
            return true;
        }

    };

};

#endif