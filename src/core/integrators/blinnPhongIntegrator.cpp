#include "blinnPhongIntegrator.hpp"
#include "blinn_phong_material.hpp"
#include "common.hpp"
#include "light.hpp"
#include <algorithm>
#include <memory>
#include <optional>

namespace rt {

    std::optional<RGBColor> BlinnPhongIntegrator::Li(const Ray& ray, const rt::Scene& scene) const {
        RGBColor L(0, 0, 0);
        rt::Surfel isect;
        if(!scene.intersect(ray, &isect)){
            return std::nullopt;
        }
        if (dot(ray.getDirection(), isect.n) > 0) {
           return std::nullopt;
        }

        std::shared_ptr<BlinnPhongMaterial> fm{nullptr};

        fm = std::dynamic_pointer_cast<BlinnPhongMaterial>( isect.primitive->get_material());

        RGBColor ka = fm->ka();
        RGBColor kd = fm->kd();
        RGBColor ks = fm->ks();

        if(!fm)return RGBColor{0, 0, 0};

        for(auto& light : scene.lights){
            Vec3 wi;
            auto Li =  light->sample_Li(isect, &wi);
            if(light->flag == light_flag_e::ambient){
                L = L + ka * Li;
            }
            else{
                auto n = isect.n;
                n.mk_unit_vec();
                auto l = wi;
                l.mk_unit_vec();
                auto v = -ray.getDirection();
                v.mk_unit_vec();
                auto h = v + l;
                h.mk_unit_vec();

                double diff_factor = std::max(0.0, dot(n, l));
                auto diffuse = kd * diff_factor;

                RGBColor specular{0, 0, 0};

                if (diff_factor > 0.0) {
                    specular = ks * std::pow(std::max(0.0, dot(n, h)), fm->gg());
                }

                L = L + Li * (diffuse + specular);
            }

        }


        return RGBColor(L.red, L.green, L.blue, "spectre");
    }

};
