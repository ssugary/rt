#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "light.hpp"

namespace rt {
    class PointLight : public Light {
        private:
            Point3 p;
            Vec3 attenuation;
        public:
            
            double att{1};

            PointLight(Point3 p, RGBColor intensity, RGBColor scale, Vec3 attenuation) :
                            Light(intensity, scale), p(p), attenuation(attenuation) {flag = light_flag_e::point;};
            RGBColor sample_Li( const Surfel& hit, Vec3* wi  ) override;
    };
}


#endif