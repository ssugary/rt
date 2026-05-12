#ifndef AMBIENT_LIGHTS_HPP
#define AMBIENT_LIGHTS_HPP

#include "light.hpp"

namespace rt {
    class AmbientLight : public Light {
        public:

            AmbientLight(RGBColor intensity, RGBColor scale) :
                            Light(intensity, scale) {flag = light_flag_e::ambient;};
            RGBColor sample_Li( const Surfel& hit, Vec3* wi, VisibilityTester* vis ) override;
    };
}

#endif
