#include "spot_light.hpp"
#include "common.hpp"
#include "ssmath/vec3.hpp"
#include "surfel.hpp"
#include "visibilityTester.hpp"
#include <cmath>

namespace rt {

    RGBColor SpotLight::sample_Li(const Surfel &hit, Vec3 *wi, VisibilityTester* vis){
    
        Vec3 dir = spot_pos - hit.p;
        Surfel lpos; lpos.p = spot_pos;
        *vis = VisibilityTester(hit, lpos);

        double dist = dir.length();

        att = 1.0f / (attenuation[0] + dist * attenuation[1] + dist * dist * attenuation[2]);

        *wi = unit_vec(dir);

        // No construtor ou num método de inicialização:

        Degrees angle = std::acos(dot(-(*wi), spot_axis)) * (180.0 / M_PI);

        double spot = 1.0;

        if(angle >= cutoff_angle){
            return RGBColor();
        }
        else if(angle >= falloff_angle){
            spot = (cutoff_angle - angle)/(cutoff_angle - falloff_angle);
        }
        
        return intensity * scale * att * spot;
        
    }

};