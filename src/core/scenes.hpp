
#ifndef SCENES_HPP
#define SCENES_HPP

#include "background.hpp"
#include "primitive.hpp"
#include "surfel.hpp"
#include "ray.hpp"
#include <memory>

#ifndef LIGHT_HPP
namespace rt{
    class Scene;
}
#include "light/light.hpp"
#endif  //< LIGHT_HPP

namespace rt {

class Scene {
    //=== Public data
    public:
        std::vector<std::shared_ptr<Light>> lights; // list of lights
        std::shared_ptr< Background > background; //< The background object.
    private:
        std::shared_ptr<Primitive> aggregate;     //< The scene graph of objects, acceleration structure.
        

    //=== Public interface
    public:
        Scene( std::shared_ptr<Primitive> ag, std::shared_ptr< Background > bkg, std::vector<std::shared_ptr<Light>> lights)
             : lights(lights), background{bkg}, aggregate{ag}
        {}
        /// Determines the intersection info; return true if there is an intersection.
        bool intersect( const Ray& r, Surfel *isect ) const;
        
        /*! A faster version that only determines whether there is an intersection or not;
         * it doesn't calculate the intersection info.
         */
        bool intersect_p( const Ray& r ) const;
};

} // namespace rt
#endif
