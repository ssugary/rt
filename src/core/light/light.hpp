#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "common.hpp"
#include "surfel.hpp"

#ifndef SCENES_HPP
namespace rt{
    class Light;
}
#include "scenes.hpp"
#endif  //< SCENES_HPP

namespace rt{

    enum class light_flag_e : std::uint8_t {
        point = 1,
        directional = 2,
        area = 4,
        ambient = 8,
        spot = 16
    };

    class Light {
        protected:
            RGBColor intensity;
            RGBColor scale;
        public:
            light_flag_e flag;

            virtual ~Light() = default;
            Light(RGBColor intensity, RGBColor scale) : intensity(intensity), scale(scale){};

            virtual rt::RGBColor sample_Li( const Surfel& hit, Vec3* wi  ) = 0;

            virtual void preprocess( const Scene & ) {};


    };

}

#endif