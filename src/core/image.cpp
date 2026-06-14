#include <cstdint>
#include <fstream>

#include "image.hpp"

namespace rt {

/// Saves an image as a **binary** PPM file.
bool save_ppm6(const std::vector<RGBColor>& data, Resolution w, Resolution h,
               const std::string &file_name_, bool gamma) {
  std::ofstream ofs(file_name_, std::ios::out | std::ios::binary);

  if (!ofs.is_open()) {
    std::cerr << "Error on create the file" + file_name_ << '\n';
    return false;
  }

  ofs << "P6\n" << w << ' ' << h << '\n';
  ofs << "255\n";

  std::vector<char> buffer;
  buffer.reserve(w * h * 3);

  for (const auto &color : data) {
    double r = std::clamp(color.red, 0.0, 1.0);
    double g = std::clamp(color.green, 0.0, 1.0);
    double b = std::clamp(color.blue, 0.0, 1.0);
    if(gamma){
      r = apply_gamma(r);
      g = apply_gamma(g);
      b = apply_gamma(b);
    }
    buffer.push_back(static_cast<char>(r * 255.0));
    buffer.push_back(static_cast<char>(g * 255.0));
    buffer.push_back(static_cast<char>(b * 255.0));
  }

  ofs.write(buffer.data(), buffer.size());
  ofs.close();

  return true; // STUB
}

/// Saves an image as a **ascii** PPM file.
bool save_ppm3(const std::vector<RGBColor>& data, Resolution w, Resolution h,
               const std::string &file_name_, bool gamma) {

  std::ofstream ofs(file_name_);

  if (!ofs.is_open()) {
    std::cerr << "Error on create the file " + file_name_ << '\n';
    return false;
  }
  ofs << "P3\n" << w << ' ' << h << "\n255\n";

  for (auto &color : data) {
    double r = std::clamp(color.red, 0.0, 1.0);
    double g = std::clamp(color.green, 0.0, 1.0);
    double b = std::clamp(color.blue, 0.0, 1.0);

    if(gamma){
      r = apply_gamma(r);
      g = apply_gamma(g);
      b = apply_gamma(b);
    }

    ofs << (int) r * 255.0 << ' ' << (int) g * 255.0 << ' ' << (int) b * 255.0 << '\n';
  }

  ofs.close();
  return true; // STUB
}

bool save_png(const std::vector<RGBColor>& data, Resolution w, Resolution h,
              const std::string &file_name_, bool gamma) {

  vector<uint64_t> img;
  img.reserve(w * h * 4);

  for (auto &color : data) {
    double r = std::clamp(color.red, 0.0, 1.0);
    double g = std::clamp(color.green, 0.0, 1.0);
    double b = std::clamp(color.blue, 0.0, 1.0);
    
    if(gamma){
      r = apply_gamma(r);
      g = apply_gamma(g);
      b = apply_gamma(b);
    }

    img.push_back(static_cast<uint64_t>(r * 255.0));
    img.push_back(static_cast<uint64_t>(g * 255.0));
    img.push_back(static_cast<uint64_t>(b * 255.0));
    img.push_back(255); // blk = 255.
  }

  vector<unsigned char> chars(img.begin(), img.end());

  unsigned error = lodepng::encode(file_name_, chars, w, h);
  if (error) {
    std::cout << "encoder error " << error << ": " << lodepng_error_text(error)
              << "\n";
    return false;
  } else {
    std::cout << "image generated in: " << file_name_ << "\n";
  }
  return true; // STUB
}

} // namespace rt
