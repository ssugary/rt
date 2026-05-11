#ifndef DIRECTIONAL_LIGHTS_HPP
#define DIRECTIONAL_LIGHTS_HPP

#include "common.hpp"
#include "light.hpp"

namespace rt {
    class DirectionalLight : public Light {
        private:
            Vec3 direction;
        public:

            DirectionalLight(Vec3 direction, RGBColor intensity, RGBColor scale) :
                            Light(intensity, scale), direction(direction) {flag = light_flag_e::directional;};
            RGBColor sample_Li( const Surfel& hit, Vec3* wi  ) override;
    };
}

#endif
