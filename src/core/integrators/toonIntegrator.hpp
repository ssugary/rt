#ifndef TOON_INTEGRATOR_HPP
#define TOON_INTEGRATOR_HPP

#include "integrator.hpp"
namespace rt{
    class ToonIntegrator : public SamplerIntegrator {
        private:
            std::vector<double> mapping_interval;
            int n_intervals;
        public:
            ToonIntegrator(std::shared_ptr<Camera> cam, std::vector<double> mapping_interval, int n_intervals, int max_depth) 
                          : SamplerIntegrator(cam, max_depth), mapping_interval(mapping_interval), n_intervals(n_intervals){};
            std::optional<RGBColor> Li(const Ray& ray, const Scene& scene, int depth) const override;
    };

}



#endif