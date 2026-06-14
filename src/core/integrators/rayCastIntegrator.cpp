#include "rayCastIntegrator.hpp"
namespace rt{
std::optional<rt::RGBColor> RayCastIntegrator::Li(const Ray& ray, const rt::Scene& scene,const int& depth) const {

    rt::RGBColor L(0,0,0); // The radiance
    // Find closest ray intersection or return background radiance.
    rt::Surfel isect; // Intersection information.

    if (!scene.intersect(ray, &isect)) {
        return std::nullopt;
    }
    // Some form of determining the incoming radiance at the ray's origin.
    // Polymorphism in action.
    std::shared_ptr<rt::Material> fm{nullptr};
    
    fm = std::dynamic_pointer_cast<rt::FlatMaterial>( isect.primitive->get_material() );
    // Assign diffuse color to L.

    if(fm){
        L = fm->kd();
    }
    else{
        L = rt::RGBColor(255, 0, 255);
    }
    return L;

}
}
