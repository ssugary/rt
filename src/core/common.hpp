#ifndef COMMON_HPP
#define COMMON_HPP

#include "ray.hpp"
#include <ssmath/ssmath.hpp>
#include <array>
#include <cstdio>
#include <optional>
#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

using byte = uint16_t;

using std::cout;
using std::endl;

using std::array;
using std::vector;
using std::string;
using Vec3 = vec3<double>;
// Corner indexes.
enum Corners_e {
  BOTTOM_LEFT = 0,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_RIGHT,
};

namespace rt {

struct Pixel {
	u_int64_t x;
	u_int64_t y;
};

struct RunningOptions {
	std::optional<array<Pixel,2>> crop_region;
	bool quick{false};
	bool verbose{false};
	string outfile = "";
	string scene;
};

struct RGBColor {
  double red;
  double green;
  double blue;

  RGBColor() : red(0), green(0), blue(0) {};
  RGBColor(Vec3 color, std::string color_type){
     *this = RGBColor(color[0], color[1], color[2], color_type);
  };
  RGBColor(RGBColor color, std::string color_type){
     *this = RGBColor(color.red, color.green, color.blue, color_type);
  };
  RGBColor(double red, double green, double blue, std::string color_type){
    
    if(color_type == "rgb"){
      this->red =  red / 255.0;
      this->green = green / 255.0;
      this->blue =  blue / 255.0;
    }
    else if (color_type == "spectre"){
      this->red = red;
      this->green =  green;
      this->blue = blue;
    }
  }

  RGBColor(double red, double green, double blue) : red(red), green(green), blue(blue) {};

  RGBColor operator*(const double& t  )const {return RGBColor(red * static_cast<double>(t), green * static_cast<double>(t), blue * static_cast<double>(t));}
  RGBColor operator*(const RGBColor& c) const{return RGBColor(red * c.red, green * c.green, blue * c.blue);};
  RGBColor operator+(const RGBColor& c)const {return RGBColor(red + c.red, green + c.green, blue + c.blue);};
  bool operator==(const RGBColor& c) const {return red == c.red && green == c.green && blue == c.blue;};
  bool operator!=(const RGBColor& c) const {return !(*this == c);};


  double&    operator[](const size_t index){
    if(index == 0)return red;
    if(index == 1)return green;
    return blue;
  };

  double operator[](const size_t index) const {
    if(index == 0)return red;
    if(index == 1)return green;
    return blue;
  };
};

// @author = Sam Altman's child
// ===
inline std::istream& operator>>(std::istream& is, rt::RGBColor& color) {
    return is >> color.red >> color.green >> color.blue;
}
inline std::ostream& operator<<(std::ostream& os, const rt::RGBColor& color) {
    return os << color.red << " " << color.green << " " << color.blue;
}
// ===

} // namespace rt

#endif //COMMON_HPP
