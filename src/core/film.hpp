#ifndef FILM_HPP
#define FILM_HPP

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common.hpp"
#include "paramset.hpp"
#include "image.hpp"

using std::unordered_map;
using std::vector;

using Resolution = uint16_t;

namespace rt {

class Film {

public:
enum class ImageType_e {
  PNG = 0,
  PPM3,
  PPM6,
};

enum class FilmType_e {
  IMAGE = 0,
};

  Film(const Resolution &w, const Resolution &h, const string &filename,
       FilmType_e film_type, ImageType_e image_type, bool gamma_corrected = false);
  ~Film() = default;

  Resolution width() const { return m_x_res; };
  Resolution height() const { return m_y_res; };

  void add(const Pixel& p, const RGBColor& c);
  void write_image() const;

private:
  // The matrix of pixels itself, starting from TL until BR
  ImageType_e m_img_type;
  FilmType_e m_film_type;

  // Buffer who storage all the colors in the scene
  std::vector<RGBColor> color_buffer;

  // The dimentions of the final image
  Resolution m_x_res;
  Resolution m_y_res;

  string m_filename;
  bool m_gamma_correction;
};

// @author = Selan Santos
// ===
/// Factory creation
std::unique_ptr<Film> create_film(const ParamSet& ps);
string handles_filename(const ParamSet& ps);
// gc::Bounds2f handles_cropwindow(const ParamSet& ps);
std::pair<Resolution, Resolution> handles_dimensions(const ParamSet& ps);
// ===

} // namespace rt

#endif // FILM_HPP
