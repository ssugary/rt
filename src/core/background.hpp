#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <array>
#include <initializer_list>

#include "common.hpp"
#include "paramset.hpp"

using d = double;
using real_type = double;

using std::array;

enum BackgroundType_e {
  COLORS = 0,
};

namespace rt {

class Background {
public:
  static constexpr byte max_channel_value{255};
  // Ctro receives a list of four colors, for each corner.
  Background(const std::array<RGBColor, 4> &colors);
  // Dtro
  ~Background() = default;

  // Sample and returns a color, based on the raster coordinate.
  RGBColor sample(const real_type& u, const real_type& v) const;

  // Return the linearly interpolated color in [A;B], based on the parameter
  static RGBColor linear_interpolation(const RGBColor &A, const RGBColor &B,
                                const double& t);

private:
  // Each corner has a color associated with.
  array<RGBColor, 4> m_corners{RGBColor(), RGBColor(), RGBColor(), RGBColor()};
  BackgroundType_e m_type{};

};

std::shared_ptr<Background> create_color_background(std::string_view type, const ParamSet &ps);
} // namespace rt

#endif // BACKGROUND_HPP
