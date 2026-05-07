#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "common.hpp"
#include "ssmath/ssmath.hpp"


using Mat4 = mat4<double>;
using Point3 = vec3<double>;
using Point4 = vec4<double>;
using Vec3 = vec3<double>;

namespace rt{
    class Material {
        public:
            virtual ~Material() = default;
            virtual RGBColor kd() const = 0;
            
    };
}      //< namespace rt
#endif //< MATERIAL_HPP


#ifndef FLAT_MATERIAL_HPP
#define FLAT_MATERIAL_HPP

namespace rt{
    class FlatMaterial : public Material{
        public:
            FlatMaterial(RGBColor color) : color(color){};
            FlatMaterial(Material &mat);

            RGBColor color;
            RGBColor kd() const override {return color;}
    };
}       //< namespace rt
#endif  //< FLAT_MATERIAL_HPP