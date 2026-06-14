#include <memory>
#include <utility>

#include "api.hpp"
#include "common.hpp"
#include "film.hpp"
#include "image.hpp"

namespace rt {
Film::Film(const Resolution &w, const Resolution &h, const string &filename,
           FilmType_e film_type, ImageType_e image_type, bool gamma_corrected)
    : m_img_type(image_type), m_film_type(film_type), m_x_res(w), m_y_res(h),
      m_filename(filename), m_gamma_correction(gamma_corrected) {
  color_buffer.resize(h * w);
}

/// Add the Spectrum color to image. Pixel coords comes as (x,y).
void Film::add(const Pixel &p, const RGBColor &c) {
  color_buffer[p.y() * m_x_res + p.x()] = c;
}

/// Convert Spectrum image information to RGB, compute final pixel values, write
/// image.
void Film::write_image() const {
  switch (m_img_type) {
  case ImageType_e::PNG:
    rt::save_png(color_buffer, m_x_res, m_y_res, m_filename, m_gamma_correction);
    break;
  case ImageType_e::PPM3:
    rt::save_ppm3(color_buffer, m_x_res, m_y_res, m_filename, m_gamma_correction);
    break;
  case ImageType_e::PPM6:
    rt::save_ppm6(color_buffer, m_x_res, m_y_res, m_filename, m_gamma_correction);
    break;
  default:
    break;
  }
}

/// Chooses the filename based on the CLI and scene file info.
/// CLI filename prevail
string handles_filename(const ParamSet &ps) {
  string filename = ps.retrieve<string>("filename", "out.png");

  return filename;
}

/// Parses the dimensions of the film from the ParamSet.
std::pair<Resolution, Resolution> handles_dimensions(const ParamSet &ps) {
  std::pair<Resolution, Resolution> dimensions;

  int x = ps.retrieve<int>("x_res", -1);
  int y = ps.retrieve<int>("y_res", -1);
  
  if(x == -1){
    x = ps.retrieve<int>("w_res", -1);
    if(x == -1){
      x = 1280;
    }
  }
  if(y == -1){
    y = ps.retrieve<int>("h_res", -1);
    if(y == -1){
      y = 720;
    }
  }
  dimensions = {x, y};
  // Quick render?
  if (API::m_run_options.quick) {
    // decrease resolution.
    dimensions.first = std::max(1, dimensions.first / 4);
    dimensions.second = std::max(1, dimensions.second / 4);
  }
  return dimensions;
}

/// Creates and returns a `Film` objected based on the `ParamSet` provided.
std::unique_ptr<Film> create_film(const ParamSet &ps) {
#ifdef DEBUG
  std::cout << ">>> Inside create_film()\n";
#endif
  //==[1] Choose the filename. The CLI prevails.
  string filename = API::m_run_options.outfile;

  if (API::m_run_options.outfile.empty()) {
    filename = handles_filename(ps);
  }

  //==[2] Define the crop window information.
  // auto crop_window = handles_cropwindow(ps);

  //==[3] Retrieve film dimensions and handles quick_render option.
  auto dimensions = handles_dimensions(ps);

  //==[4] Retrieve image type.
  std::unordered_map<string, Film::ImageType_e> image_type_opts{
      {"png", Film::ImageType_e::PNG},
      {"ppm3", Film::ImageType_e::PPM3},
      {"ppm6", Film::ImageType_e::PPM6},
      {"ppm", Film::ImageType_e::PPM6},
  };
  auto img_type{image_type_opts[ps.retrieve<string>("img_type", "png")]};

  std::unordered_map<string, Film::FilmType_e> film_type_opts{
      {"image", Film::FilmType_e::IMAGE},
  };

  //==[5] Retrieve film type.
  auto film_type{film_type_opts[ps.retrieve<string>("type", "image")]};

  //==[6] Get gamma correction request.
  bool apply_gamma_correction = ps.retrieve<bool>("gamma_corrected", false);

#ifdef DEBUG
  std::cout << "================================================\n";
  std::cout << ">>> create_film() - film parameters are:\n";
  std::cout << "    - filename: " << std::quoted(filename) << "\n";
  std::cout << "    - crop window: " << crop_window << "\n";
  std::cout << "    - w_res: " << dimensions.x << "\n";
  std::cout << "    - h_res: " << dimensions.y << "\n";
  std::cout << "    - film type: " << ps.retrieve<string>("type", "image")
            << "\n";
  std::cout << "    - image type: " << ps.retrieve<string>("img_type", "png")
            << "\n";
  std::cout << "    - gamma correction: " << std::boolalpha
            << apply_gamma_correction << "\n";
  std::cout << "================================================\n";
#endif

  return std::make_unique<Film>(dimensions.first, dimensions.second, filename,
                                film_type, img_type, apply_gamma_correction);
}
} // namespace rt
