#ifndef API_HPP
#define API_HPP

#include <memory>
#include <unordered_map>

#include "light/include_lights.hpp"
#include "scenes.hpp"
#include "background.hpp"
#include "camera.hpp"
#include "error.hpp"
#include "film.hpp"
#include "parser.hpp"
#include "include_integrator.hpp"
#include "include_primitive.hpp"
#include "include_material.hpp"

namespace rt {

struct RenderOptions {
  std::shared_ptr<Background> background;
  std::shared_ptr<Camera> camera;
  std::unique_ptr<Integrator> integrator;
  std::unique_ptr<rt::Scene> scene;

  std::vector<std::shared_ptr<Light>> light_sources;
  std::vector<std::shared_ptr<Primitive>> elements;
  std::unordered_map<string, ParamSet> setup_params;

  std::map<std::string, std::shared_ptr<Material>> material_memory;
  std::shared_ptr<Material> current_material;

};

class API {
public:
  enum APIState {
    Uninitialized,
    Initializing,
    Setup,
    World,
  };

  /// The infrastructure to render a scene (camera, integrator, etc.).
  static std::unique_ptr<RenderOptions> m_render_options;
  /// Current API state
  static APIState m_api_state;
  // Stores the CLI options that the
  static RunningOptions m_run_options;

  // setup the ray tracer
  static void init_engine(const RunningOptions &opt);
  // calls the static function parse(opt.file_name)
  static void run();
  // ray tracing loop
  static void render();
  // frees all the resources previously allocated
  static void clean_up();

  // ParamSet methods to use on objects
  static void camera(const ParamSet &ps);
  static void look_at(const ParamSet &ps);
  static void background(const ParamSet &ps);
  static void material(const ParamSet &ps);
  static void world_begin(const ParamSet &ps);
  static void world_end(const ParamSet &ps);
  static void film(const ParamSet &ps);
  static void object(const ParamSet &ps);
  static void integrator(const ParamSet &ps);
  static void make_named_material(const ParamSet &ps);
  static void named_material(const ParamSet &ps);
  static void light_source(const ParamSet& ps);

  // Methods that create the objects based on paramset's data
  static std::unique_ptr<Camera> make_camera(const ParamSet &camera,
                                             const ParamSet &look_at,
                                             const Resolution width,
                                             const Resolution height);
  static std::unique_ptr<Film> make_film(const ParamSet &ps);
  static std::unique_ptr<Integrator> make_integrator(const ParamSet& ps);
private:
  static bool check_in_initialized_state(std::string_view func_name);
  static bool check_in_setup_block_state(std::string_view func_name);
  static bool check_in_world_block_state(std::string_view func_name);

  static void soft_engine_reset();
  static void hard_engine_reset();
};

} // namespace rt

#endif // API_HPP