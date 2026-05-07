#include "blinnPhongIntegrator.hpp"
#include "blinn_phong_material.hpp"
#include "common.hpp"
#include "light.hpp"

namespace rt {

    std::optional<RGBColor> BlinnPhongIntegrator::Li(const Ray& ray, const rt::Scene& scene, int depth) const {
        rt::RGBColor L(0, 0, 0);
        rt::Surfel isect;
        if(!scene.intersect(ray, &isect)){
            return std::nullopt;
        }
        // TODO [2] SPECIAL SITUATION: IF THE RAY HITS THE SURFACE FROM "BEHIND" (INSIDE),
        //     WE DO NOT COLOR.

        std::shared_ptr<Material> fm{nullptr};

        fm = std::dynamic_pointer_cast<BlinnPhongMaterial>( isect.primitive->get_material());
        // TODO
        // for(auto light : scene.lights){
        //     Vec3 wi;
        //     auto Li = light->sample_Li(isect, &wi);
        //     if(light->flag == light_flag_e::ambient){
        //         L = L + fm->kd() * Li;
        //     }
        //     else{
                
        //     }

        // }

    }

};
