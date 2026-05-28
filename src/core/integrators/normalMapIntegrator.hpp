#include "integrator.hpp"

namespace rt{
class NormalMapIntegrator : public SamplerIntegrator {
    public:
        NormalMapIntegrator(std::shared_ptr<rt::Camera> cam, int max_depth) : SamplerIntegrator(cam, max_depth) {}
        std::optional<rt::RGBColor> Li(const Ray& ray, const rt::Scene& scene, int depth) const override;
};
}