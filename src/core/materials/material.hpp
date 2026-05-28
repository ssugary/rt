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
        protected:
            RGBColor mirror;
        public:
            Material(RGBColor& mirror) : mirror(mirror) {};
            virtual ~Material() = default;
            virtual RGBColor kd() const = 0;
            virtual RGBColor km() const = 0;        

            
    };
}      //< namespace rt
#endif //< MATERIAL_HPP


#ifndef FLAT_MATERIAL_HPP
#define FLAT_MATERIAL_HPP

namespace rt{
    class FlatMaterial : public Material{
        public:
            FlatMaterial(RGBColor color, RGBColor mirror = RGBColor()) : Material(mirror), color(color){};
            FlatMaterial(Material &mat);

            RGBColor color;
            RGBColor kd()    const override {return color;}
            RGBColor km()const override {return mirror;};   

    };
}       //< namespace rt
#endif  //< FLAT_MATERIAL_HPP