#include "point_light.hpp"


namespace rt {

RGBColor PointLight::sample_Li(const Surfel& hit, Vec3* wi){
    Vec3 direction = p - hit.p;
    *wi = unit_vec(direction);
    
    double dist = direction.length();
    att = 1.0f / (attenuation[0] + dist * attenuation[1] + dist * dist * attenuation[2]);

    return intensity * scale * att;
}

};