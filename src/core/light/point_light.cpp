#include "point_light.hpp"
#include "CLI11/CLI11.hpp"
#include "visibilityTester.hpp"


namespace rt {

RGBColor PointLight::sample_Li(const Surfel& hit, Vec3* wi, VisibilityTester* vis){
    Vec3 direction = p - hit.p;
    *wi = unit_vec(direction);

    Surfel lpos; lpos.p = p;

    *vis = VisibilityTester(hit, lpos);
    
    double dist = direction.length();
    att = 1.0f / (attenuation[0] + dist * attenuation[1] + dist * dist * attenuation[2]);

    return intensity * scale * att;
}

};