#include "ambient_light.hpp"
#include "common.hpp"
#include "visibilityTester.hpp"

namespace rt{

    RGBColor AmbientLight::sample_Li( const Surfel& hit, Vec3* wi, VisibilityTester* vis ){
        *wi = {0, 0, 0};
        return intensity * scale;
    }

}