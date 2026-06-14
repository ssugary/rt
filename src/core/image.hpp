#ifndef IMAGE_HPP
# define IMAGE_HPP

// @author = Sam Altman's child
// ===
# include <cstdint>
# include <string>
# include <vector>

# include "common.hpp"
# include "lodepng/lodepng.h"

using Resolution = uint16_t;


namespace rt {

/// auxiliar funcition that aplies gamma correction  
inline double apply_gamma(double value, double gamma = 2.2) {
    if (value <= 0.0) return 0.0;
    if (value >= 1.0) return 1.0;
    return std::pow(value, 1.0 / gamma);
}
/// Routines to write images to a file.
bool save_ppm6(const std::vector<RGBColor>&, Resolution, Resolution, const std::string& = "image.ppm", bool = 0);

/// Saves an image as a **ascii** PPM file.
bool save_ppm3(const std::vector<RGBColor>&, Resolution, Resolution, const std::string& = "image.ppm", bool = 0);

/// Saves an image as a PNG file.
bool save_png(const std::vector<RGBColor>&, Resolution, Resolution, const std::string& = "image.png", bool = 0);
}  // namespace rt
// ===
#endif
