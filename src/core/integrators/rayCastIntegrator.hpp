#include "integrator.hpp"

namespace rt{
class RayCastIntegrator : public SamplerIntegrator {
    public:
        RayCastIntegrator(std::shared_ptr<rt::Camera> cam, int max_depth) : SamplerIntegrator(cam, max_depth) {}
        std::optional<rt::RGBColor> Li(const Ray& ray, const rt::Scene& scene,const int& depth) const override;
};
}