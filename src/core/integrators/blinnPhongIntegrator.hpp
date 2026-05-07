#ifndef BLINN_PHONG_INTEGRATOR_HPP
#define BLINN_PHONG_INTEGRATOR_HPP

#include "include_material.hpp"
#include "integrator.hpp"

namespace rt{
    class BlinnPhongIntegrator : public SamplerIntegrator {
        private:
            double depth;
        public:
            BlinnPhongIntegrator(std::shared_ptr<rt::Camera> cam, double depth = 1.0) : SamplerIntegrator(cam), depth(depth){}
            std::optional<rt::RGBColor> Li(const Ray& ray, const rt::Scene& scene) const override;
    };

}
#endif