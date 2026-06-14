#include "common.hpp"
#include "integrator.hpp"
#include <cmath>

namespace rt{

class DepthMapIntegrator : public SamplerIntegrator {
	private:
		double m_zmin;
		double m_zmax;

		double m_zmin_abs = INFINITY;
		double m_zmax_abs = 0.0;

		RGBColor m_near_color;
		RGBColor m_far_color;

    public:
        DepthMapIntegrator(std::shared_ptr<rt::Camera> cam, double zmin, double zmax, RGBColor near, RGBColor far, int max_depth) : SamplerIntegrator(cam, max_depth), m_zmin(zmin), m_zmax(zmax), m_near_color(near), m_far_color(far) {}

        std::optional<rt::RGBColor> Li(const Ray& ray, const rt::Scene& scene, const int& depth) const override;
        virtual void render(const rt::Scene& scene) override;
        virtual void preprocess(const rt::Scene& scene) override;
};

} // namespace rt
