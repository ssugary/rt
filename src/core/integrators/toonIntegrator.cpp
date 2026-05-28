#include "toonIntegrator.hpp" 
#include "common.hpp"
#include "light.hpp"
#include "toon_material.hpp"
#include "visibilityTester.hpp"
#include <algorithm>
#include <memory>
#include <optional>
#include <cmath> 

namespace rt {

    std::optional<RGBColor> ToonIntegrator::Li(const Ray& ray, const rt::Scene& scene, int depth) const {
        RGBColor L(0, 0, 0);
        Surfel isect;
        if(!scene.intersect(ray, &isect)){
            return std::nullopt;
        }
        if (dot(ray.getDirection(), isect.n) > 0) {
           return std::nullopt;
        }


        std::shared_ptr<ToonMaterial> fm = std::dynamic_pointer_cast<ToonMaterial>(isect.primitive->get_material());
        if(!fm) return RGBColor{0, 0, 0};

        vector<RGBColor> gm = fm->gm();
        
        auto n = isect.n;
        n.mk_unit_vec();
        auto v = -ray.getDirection();
        v.mk_unit_vec();

        if(dot(n, v) <= 0.2){
            return fm->kd();
        }
        
        for(auto& light : scene.lights){
            VisibilityTester vis;
            Vec3 wi;
            auto Li = light->sample_Li(isect, &wi, &vis);

            if(light->flag == light_flag_e::ambient){
                L = L + gm[0] * Li;
                continue;
            }

            if(!vis.unoccluded(scene)){
                L = L + gm[0] * Li;
                continue; 
            }
            
            auto n = isect.n;
            n.mk_unit_vec();
            auto l = wi;
            l.mk_unit_vec();


            double cos = std::max(0.0, dot(n, l));
            double angle = std::acos(cos) * (180.0/M_PI);
            
            size_t idx = mapping_interval.size() - 1;

            for(size_t i{0}; i < mapping_interval.size(); ++i){
                if(angle < mapping_interval[i]){
                    idx = i;
                    break;
                }
            }
            
            size_t color_idx = mapping_interval.size() - idx - 1;
            if(color_idx >= gm.size()){
                color_idx = gm.size() - 1;
            }
            L = L + Li * gm[color_idx];
        }

        return RGBColor(L.red, L.green, L.blue, "spectre");
    }
};