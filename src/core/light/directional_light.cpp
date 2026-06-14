#include "directional_light.hpp"

namespace rt {

    RGBColor DirectionalLight::sample_Li(const Surfel& hit, Vec3* wi, VisibilityTester* vis ){
        *wi = -direction;
        wi->mk_unit_vec();

        Surfel lpos;

        // lpos.p = hit.p + (*wi) * world_radius;

        *vis = VisibilityTester(hit, *wi);

        return intensity * scale ;
    }

};