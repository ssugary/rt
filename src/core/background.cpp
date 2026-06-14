#include <array>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>

#include "background.hpp"
#include "common.hpp"
#include "error.hpp"
#include "paramset.hpp"

const d width = 400;
const d height = 200;
const d max = 255;

namespace rt {

Background::Background(const std::array<RGBColor, 4> &colors) {
  for (size_t i = 0; i < 4; i++) {
    m_corners[i] = colors[i];
  }
}

RGBColor Background::linear_interpolation(const RGBColor &A, const RGBColor &B,
                                          const double& t) {            
  return RGBColor{static_cast<double>((1 - t) * A.red + t * B.red),
                  static_cast<double>((1 - t) * A.green + t * B.green),
                  static_cast<double>((1 - t) * A.blue + t * B.blue)};
};

RGBColor Background::sample(const real_type& u, const real_type& v) const {  
  const auto bottom_horizontal =
      linear_interpolation(m_corners[BOTTOM_LEFT], m_corners[BOTTOM_RIGHT], u);
  const auto top_horizontal =
      linear_interpolation(m_corners[TOP_LEFT], m_corners[TOP_RIGHT], u);

  const auto bilerp =
      linear_interpolation(top_horizontal, bottom_horizontal, v);

  return bilerp;
};

// @author = Selan Santos
// ===
std::shared_ptr<Background> create_color_background(std::string_view type, const ParamSet &ps) {
  // List of name ids for each corner of the background.
  std::array<std::string, 4> corner_name{"tl", "bl", "br", "tr"};
  RGBColor black = RGBColor();

  // Possible colored background types: single_color, 4_colors.
  if (type == "single_color") {
    // The tag:
    // <background type="single_color" color="153 204 255"/>
    // default color is black
    auto color_type = ps.retrieve<std::string>("color_type", "rgb");
    RGBColor single_color{ps.retrieve<RGBColor>("color", black), color_type};
    return std::make_shared<Background>(
        std::array{single_color, single_color, single_color, single_color});
  }
  if (type == "4_colors") {
    // List of color from the scene to be passed onto the constructor.
    std::array<RGBColor, 4> color_list;
    // The tag:
    // <background type="4_colors"  bl="0 0 51" tl="0 255 51" tr="255 255 51"
    // br="255 0 51" />
    size_t idx{0};
    for (const auto &label : corner_name) {
      // black by default again
      auto color_type = ps.retrieve<std::string>("color_type", "rgb");
      RGBColor color{ps.retrieve<RGBColor>(label, black), color_type};
      
      color_list[idx++] = color;
    }
    return std::make_shared<Background>(color_list);
  }
  // If we got here it means we received an invalid colored background
  // specification.
  std::ostringstream oss;
  oss << "create_color_background(): Unknown type of colored background "
         "specified "
      << std::quoted(type) << ", using black background.";
  WARNING(oss.str());
  return std::make_shared<Background>(std::array{black, black, black, black});
}
// ===

} // namespace rt
