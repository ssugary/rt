#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include "common.hpp"
#include "light.hpp"

namespace rt {
class SpotLight : public Light {
  using Degrees = double;

private:
  Point3 spot_pos;
  Vec3 spot_axis;
  Vec3 attenuation;
  Degrees cutoff_angle;
  Degrees falloff_angle;
  int world_radius;
  
public:

  double att{1};
  

  SpotLight(Point3 pos, Vec3 axis, Degrees cutoff, Degrees falloff, int world_radius,
            RGBColor intensity, RGBColor scale, Vec3 attenuation)
      : Light(intensity, scale), spot_pos(pos), spot_axis(axis), attenuation(attenuation),
        cutoff_angle(cutoff), falloff_angle(falloff), world_radius(world_radius) {
    flag = light_flag_e::spot;
  };
  RGBColor sample_Li(const Surfel &hit, Vec3 *wi) override;
};
} // namespace rt

#endif
