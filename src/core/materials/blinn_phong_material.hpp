#ifndef BLINN_PHONG_MATERIAL_HPP
#define BLINN_PHONG_MATERIAL_HPP

#include "common.hpp"
#include "material.hpp"

namespace rt{

    class BlinnPhongMaterial : public Material{
        private:
            RGBColor diffuse;   //< color that indicates how much diffuse color is reflected.
            RGBColor specular;  //< color that represents the color of the specular highlights.
            RGBColor ambient;   //< color that represents how much the incoming light is reflected.
            double glossiness;  //< value that control how narrowed is the specular highlight in the scene.
        public:
        BlinnPhongMaterial(RGBColor mirror = RGBColor()) : Material(mirror), diffuse(), specular(), ambient(), glossiness() {};
        BlinnPhongMaterial(RGBColor diffuse, RGBColor specular, RGBColor ambient, double glossiness, RGBColor mirror = RGBColor()) :
                          Material(mirror), diffuse(diffuse), specular(specular), ambient(ambient), glossiness(glossiness) {};

        RGBColor         km() const override {return mirror;};   //< mirror's getter
        virtual RGBColor kd() const override {return diffuse;};  //< diffuse coeficient's getter
        virtual RGBColor ks() const {return specular;};          //< specular coeficient's getter
        virtual RGBColor ka() const {return ambient;};           //< ambient coeficient's getter
        virtual double   gg() const {return glossiness;};        //< glossiness's getter 
        


    };

}
#endif