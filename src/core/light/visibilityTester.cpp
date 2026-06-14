#include "visibilityTester.hpp"

#include "ssmath/vec3.hpp"

namespace rt {

    VisibilityTester::VisibilityTester( const Surfel& sf1, const Surfel& sf2){

        this->p0 = sf1;
        this->p1 = sf2;
        this->direction = sf2.p - sf1.p;
        this->is_infinity = false;
    }

    VisibilityTester::VisibilityTester( const Surfel& sf, const Vec3& dir){
        this->p0 = sf;
        this->direction = unit_vec(dir);
        this->p1.p = sf.p + direction;
        this->is_infinity = true;
    }

    bool VisibilityTester::unoccluded( const Scene& scene ) {
        
        Vec3 dir;
        double dist;
        if(is_infinity){
            dir = direction;
            dist = INFINITY;
        }
        else{
            Vec3 path = p1.p - p0.p;
            dist = path.length();
            dir = path/dist;
        }

        Ray shadowRay(p0.p + dir * 0.0001f, dir);
        shadowRay.setTMax(dist - 0.0001f); 

        return !scene.intersect_p(shadowRay);
    }


};