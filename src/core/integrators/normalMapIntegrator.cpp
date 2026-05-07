#include "normalMapIntegrator.hpp"

namespace rt{
std::optional<rt::RGBColor> NormalMapIntegrator::Li(const Ray& ray, const rt::Scene& scene) const {
    rt::RGBColor L(0, 0, 0);
    rt::Surfel isect;
    if(!scene.intersect(ray, &isect)){
        return std::nullopt;
    }

    std::shared_ptr<rt::Material> fm{nullptr};

    Vec3 temp = isect.n;
    temp += {1, 1, 1};
    temp /= 2;
    temp *= 255;

    fm = std::make_shared<rt::FlatMaterial>(rt::RGBColor(temp[0], temp[1], temp[2]));

    if(fm){
        L = fm->get_color();
    }
    else{
        L = rt::RGBColor(255, 0, 255);
    }

    return L;

}
}
