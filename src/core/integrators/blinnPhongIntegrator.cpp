#include "blinnPhongIntegrator.hpp"
#include "blinn_phong_material.hpp"
#include "common.hpp"
#include "light.hpp"
#include "scenes.hpp"
#include "visibilityTester.hpp"
#include <algorithm>
#include <memory>
#include <optional>

namespace rt {

    std::optional<RGBColor> BlinnPhongIntegrator::Li(const Ray& ray, const rt::Scene& scene, const int& depth) const {
        RGBColor L(0, 0, 0);
        Surfel isect;
        if(!scene.intersect(ray, &isect)){
            return std::nullopt;
        }
        if (dot(ray.getDirection(), isect.n) > 0) {
           isect.n = -isect.n;
        }

        std::shared_ptr<BlinnPhongMaterial> fm{nullptr};

        fm = std::dynamic_pointer_cast<BlinnPhongMaterial>( isect.primitive->get_material());
        if(!fm)return RGBColor{0, 0, 0};

        RGBColor ka = fm->ka();
        RGBColor kd = fm->kd();
        RGBColor ks = fm->ks();
        RGBColor km = fm->km();
        float    gg = fm->gg();

        auto v = -ray.getDirection();
        v.mk_unit_vec();
        auto n = isect.n;
        n.mk_unit_vec();
        
        for(auto& light : scene.lights){
            VisibilityTester vis;
            Vec3 wi;
            auto Li =  light->sample_Li(isect, &wi, &vis);

            
            if(light->flag == light_flag_e::ambient){
                L = L + ka * Li;
                continue;
            }

            if(!vis.unoccluded(scene)){
                continue;
            }
            
            auto l = wi;
            l.mk_unit_vec();
            auto h = v + l;
            h.mk_unit_vec();

            double diff_factor = std::max(0.0, dot(n, l));
            auto diffuse = kd * diff_factor;

            RGBColor specular{0, 0, 0};

            if (diff_factor > 0.0 && gg != 0) {
                specular = ks * std::pow(std::max(0.0, dot(n, h)), gg);
            }

            L = L + Li * (diffuse + specular);
            
        }

    
        if(depth < max_depth){
            auto rd = n * 2 * dot(n, v) - v;
            rd.mk_unit_vec();

            Ray reflected_ray = Ray(isect.p + rd * 0.0001f, rd);
            auto tempL = this->Li(reflected_ray, scene, depth + 1);

            if(tempL.has_value()){
                L = L + km * tempL.value();
            }
        }

        return RGBColor(L.red, L.green, L.blue, "spectre");
    }

};
