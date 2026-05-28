#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "camera.hpp"
#include "common.hpp"
#include "ray.hpp"
#include "scenes.hpp"
#include "visibilityTester.hpp"
#include <memory>

namespace rt{
class Integrator {
    public:
        virtual ~Integrator() = default;
        virtual void render(const rt::Scene& scene) = 0;
};

class SamplerIntegrator : public Integrator {
    protected:
        int max_depth;
        std::shared_ptr<rt::Camera> camera;
    public:
        virtual ~SamplerIntegrator() = default;
        SamplerIntegrator(std::shared_ptr<rt::Camera> cam, int max_depth) : max_depth(max_depth), camera(cam){};

        virtual std::optional<RGBColor> Li(const Ray& ray, const rt::Scene& scene, int depth) const = 0;
        virtual void render(const rt::Scene& scene) override;
        virtual void preprocess(const rt::Scene& scene) {};

};
}


#endif
