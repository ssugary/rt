#ifndef BLINN_PHONG_INTEGRATOR_HPP
#define BLINN_PHONG_INTEGRATOR_HPP

#include "include_material.hpp"
#include "integrator.hpp"

namespace rt{
    class BlinnPhongIntegrator : public SamplerIntegrator {
        public:
            BlinnPhongIntegrator(std::shared_ptr<rt::Camera> cam, int max_depth) : SamplerIntegrator(cam, max_depth){}
            std::optional<rt::RGBColor> Li(const Ray& ray, const rt::Scene& scene, int depth) const override;
    };

}
#endif