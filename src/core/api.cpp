#include <chrono>
#include <iomanip>
#include <memory>
#include <sstream>
#include <utility>

#include "api.hpp"
#include "common.hpp"
#include "spot_light.hpp"
#include "ssmath/vec3.hpp"
#include "scenes.hpp"
#include "toonIntegrator.hpp"
#include "toon_material.hpp"


namespace rt {

// @author = Selan Santos
// ===
std::unique_ptr<RenderOptions> API::m_render_options;
API::APIState API::m_api_state = API::Uninitialized;
// @author = Selan Santos
// ===
RunningOptions API::m_run_options;

void API::init_engine(const RunningOptions &opt) {
  m_run_options = opt;

  if (m_api_state != API::Uninitialized) {
    ERROR("API::init_engine() has already been called! ");
  }

  m_api_state = API::Setup;
  m_render_options = std::make_unique<RenderOptions>();
  // m_current_gs = GraphicsState();
  MESSAGE("[1] Rendering engine initiated.\n");
}

// frees all the resources previously allocated
void API::clean_up() {}

/// Check whether the current state has been intialized.
bool API::check_in_initialized_state(std::string_view func_name) {
  if (m_api_state == API::Uninitialized) {
    std::ostringstream oss;
    oss << "API::init() must be called before " << func_name << ". Ignoring...";
    ERROR(oss.str());
    return false;
  }
  return true;
}

/// Check whether the current state corresponds to setup section.
bool API::check_in_setup_block_state(std::string_view func_name) {
  check_in_initialized_state(func_name);
  if (m_api_state == API::World) {
    std::ostringstream oss;
    oss << "Rendering setup cannot happen inside World Definition block; ";
    oss << func_name << " not allowed. Ignoring...";
    ERROR(oss.str());
    return false;
  }
  return true;
}

/// Check whether the current state corresponds to the world section.
bool API::check_in_world_block_state(std::string_view func_name) {
  check_in_initialized_state(func_name);
  if (m_api_state == API::Setup) {
    std::ostringstream oss;
    oss << "Scene description must happen inside World Definition block; ";
    oss << func_name << " not allowed. Ignoring...";
    ERROR(oss.str());
    return false;
  }
  return true;
}

void API::world_begin(const ParamSet &ps) {
  check_in_setup_block_state("API::world_begin()");
  m_api_state = API::World; // correct machine state.
  hard_engine_reset();
}

/// Erase temporary engine states so that we may render another scene with the
/// same configuration.
void API::hard_engine_reset() {
  /// TODO
}

void API::light_source(const ParamSet &ps) {
  check_in_world_block_state("light_source()");

  auto type = ps.retrieve<std::string>("type", "unknown");

  if (type == "unknown") {
    ERROR("API::light_source(): Missing \"type\" specification for the light "
          "source.");
  }
  auto color_type = ps.retrieve<string>("color_type", "spectre");
  auto I = RGBColor(ps.retrieve<RGBColor>("i", rt::RGBColor()), color_type);
  auto scale = RGBColor(ps.retrieve<RGBColor>("scale", {1, 1, 1}), color_type);

  if (type == "point") {
    auto from = ps.retrieve<Point3>("from", {0, 0, 0});
    auto attenuation = ps.retrieve<Vec3>("attenuation", {1, 0, 0});
    m_render_options->light_sources.push_back(
        std::make_shared<PointLight>(from, I, scale, attenuation));
  } else if (type == "directional") {
    auto from = ps.retrieve<Point3>("from", {0, 0, 0});
    auto to = ps.retrieve<Vec3>("to", {0, 0, 0});
    auto world_radius = ps.retrieve<double>("world_radius", 0);

    auto direction = to - from;
    m_render_options->light_sources.push_back( std::make_shared<DirectionalLight>(direction, I, scale, world_radius));
  } else if (type == "spot"){

    auto from = ps.retrieve<Point3>("from", {0, 0, 0});
    auto to = ps.retrieve<Vec3>("to", {0, 0, 0});

    auto direction = unit_vec(to - from);
    
    auto cutoff = ps.retrieve<double>("cutoff");
    auto falloff = ps.retrieve<double>("falloff");
    auto world_radius = ps.retrieve<int>("world_radius", 0);
    auto attenuation = ps.retrieve<Vec3>("attenuation", {1, 0, 0});


    m_render_options->light_sources.push_back(
        std::make_shared<SpotLight>(from, direction, cutoff, falloff, world_radius, I, scale, attenuation)
	);
  } else if (type == "ambient") {
    m_render_options->light_sources.push_back(
        std::make_shared<AmbientLight>(I, scale));
  }
}
void API::film(const ParamSet &ps) {
  if (not check_in_setup_block_state("API::film()")) {
    return;
  }
  // Store the ps associated with camera for later retrieval.
  m_render_options->setup_params["film"] = ps;
  if (m_run_options.verbose) {
    auto type = ps.retrieve<std::string>("type", "unknown");
    std::cout << ">>> film type: " << std::quoted(type) << '\n';
  }
}

void API::background(const ParamSet &ps) {
  check_in_world_block_state("API::background");

  auto type = ps.retrieve<std::string>("type", "unknown");
  if (type == "unknown") {
    ERROR(
        "API::background(): Missing \"type\" specificaton for the background.");
  }
  std::shared_ptr<Background> bkg{nullptr};
  if (type == "single_color" or type == "4_colors") {
    bkg = create_color_background(type, ps);
  } else {
    WARNING(std::string{"API::background(): unknown background type \""} +
            type + std::string{"\" provided; assuming colored background."});
    bkg = create_color_background(type, ps);
  }
  // Store current background objec.
  m_render_options->background = bkg;
}
void API::integrator(const ParamSet &ps) {
  if (not check_in_setup_block_state("API::integrator")) {
    return;
  }

  m_render_options->setup_params["integrator"] = ps;

  if (m_run_options.verbose) {
    auto type = ps.retrieve<std::string>("type", "unknown");
    std::cout << ">>> integrator type: " << std::quoted(type) << '\n';
  }
}
std::unique_ptr<Film> API::make_film(const ParamSet &ps) {
  std::unique_ptr<Film> film{nullptr};
  auto film_type = ps.retrieve<string>("type", "unknown");
  if (film_type == "image") {
    film = create_film(ps);
  } else {
    WARNING(string{"Film \""} + film_type + string{"\" unknown."});
  }
  return film;
}

std::unique_ptr<Integrator> API::make_integrator(const ParamSet &ps) {
  std::unique_ptr<Integrator> inter{nullptr};
  auto integrator_type = ps.retrieve<string>("type", "flat");
  auto depth = ps.retrieve<int>("depth", 0);

  if (integrator_type == "flat") {
    inter = std::make_unique<RayCastIntegrator>(m_render_options->camera, depth);
  } else if (integrator_type == "normal_map") {
    inter = std::make_unique<NormalMapIntegrator>(m_render_options->camera, depth);
  } else if (integrator_type == "depth_map") {
    auto near = ps.retrieve<RGBColor>("near_color", RGBColor(0, 0, 0));
    auto far = ps.retrieve<RGBColor>("far_color", RGBColor(255, 255, 255));

    auto zmin = ps.retrieve<double>("zmin", 0.0);
    auto zmax = ps.retrieve<double>("zmax", 1.0);
    
    auto depth = ps.retrieve<int>("depth", 0);

    inter = std::make_unique<DepthMapIntegrator>(m_render_options->camera, zmin,
                                                 zmax, near, far, depth);
  } else if (integrator_type == "blinn_phong") {
    auto depth = ps.retrieve<int>("depth", 1);
    inter =
        std::make_unique<BlinnPhongIntegrator>(m_render_options->camera, depth);
  } else {
    WARNING(string{"Integrator \""} + integrator_type + string{"\" unknown."});
  }
  return inter;
}

// TODO: Adapt
void API::run() {
  Parser::parse_scene(m_run_options.scene);
  // [1] Parser and load scene file
  // [2] Instantiate the Camera, Film, and Background objects.
  // [3] Enter the ray tracing main loop, because parser has found the
  // `world_end` tag
};

void API::world_end(const ParamSet &ps) {
  MESSAGE(
      "====================================================================");
  MESSAGE("   Parsing Phase has ended. Rendering process starts now...");
  MESSAGE(
      "====================================================================");

  check_in_world_block_state("API::world_end()");

  // ===============================================================
  // 1) Create the integrator.
  // 2) Create the scene (requires the list of objects and background)
  // ===============================================================
  // For now, we create the film here but in the future it will be
  // instantiated somewhere else.
  // TODO: finish

  auto primitive_list = std::make_shared<PrimitiveList>();
  for (auto &obj : m_render_options->elements) {
    primitive_list->add(obj);
  }

  auto scene =
      std::make_unique<Scene>(primitive_list, m_render_options->background,
                              m_render_options->light_sources);

  std::unique_ptr<Film> film =
      make_film(m_render_options->setup_params["film"]);

  Resolution w = film->width();
  Resolution h = film->height();

  std::unique_ptr<Camera> camera =
      make_camera(m_render_options->setup_params["camera"],
                  m_render_options->setup_params["lookat"], w, h);
  if (film == nullptr) {
    ERROR("API::setup_camera(): Unable to create film.");
  } else if (camera == nullptr) {
    ERROR("API::setup_camera(): Unable to create camera.");
  } else {
    camera->film = std::move(film);
    m_render_options->camera = std::move(camera);
    m_render_options->scene = std::move(scene);
    m_render_options->integrator =
        make_integrator(m_render_options->setup_params["integrator"]);
  }

  // The scene has already been parsed and properly set up. It's time to render
  // the scene. [1] Create the integrator. [2] Create the scene. [3] Run
  // integrator if previous instantiations went ok
  bool scene_and_integrator_ok{true}; // THIS is a STUB.
  if (scene_and_integrator_ok) {
    MESSAGE("    Parsing scene successfuly done!\n");
    MESSAGE("[2] Starting ray tracing progress.\n");
    MESSAGE("    Ray tracing is usually a slow process, please be patient: \n");
    //================================================================================
    auto start = std::chrono::steady_clock::now();
    // m_integrator->render(*m_scene);
    render();
    auto end = std::chrono::steady_clock::now();
    //================================================================================
    auto diff = end - start; // Store the time difference between start and end
    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    MESSAGE("    Time elapsed: " + std::to_string(diff_sec.count()) +
            " seconds (" +
            std::to_string(
                std::chrono::duration<double, std::milli>(diff).count()) +
            " ms) \n");
  }
  // [4] Basic clean up, preparing for new rendering, in case we have
  // several scene setup + world in a single input scene file.
  m_api_state = API::Setup; // correct machine state.
}

void API::camera(const ParamSet &ps) {
  if (not check_in_setup_block_state("API::camera()")) {
    return;
  }

  auto type = ps.retrieve<std::string>("type", "unknown");
  if (type == "unknown") {
    ERROR("API::camera(): Missing \"type\" specification for the background.");
  } else if (type == "perspective") {
    auto fovy = ps.retrieve<double>("fovy", -1.0);
    if (fovy == -1.0) {
      ERROR(
          "API::camera(): Missing \"fovy\" propertie for perspective camera.");
    }
  } else if (type == "orthographic") {
    auto screen_window =
        ps.retrieve<Point4>("screen_window", Point4(-1.777, 1.777, -1.0, 1.0));
    if (screen_window == Point4(-1.777, 1.777, -1.0, 1.0)) {
      WARNING("API::camera(): Missing \"screen_window\" propertie for "
              "orthographic camera. Considering 16:9.");
    }
  }

  m_render_options->setup_params["camera"] = ps;
}

void API::material(const ParamSet &ps) {
  check_in_world_block_state("API::material");
  auto type = ps.retrieve<std::string>("type", "unknown");
  if (type == "unknown") {
    ERROR("API::material(): Missing \"type\" specification for the material.");
  }

  if (type == "flat") {
    auto color_type = ps.retrieve<std::string>("color_type", "rgb");
    auto color =
        RGBColor(ps.retrieve<RGBColor>("color", RGBColor()), color_type);
    m_render_options->current_material =
        std::shared_ptr<Material>(new FlatMaterial(color));
  } else if (type == "blinn") {
    auto color_type = ps.retrieve<std::string>("color_type", "spectre");
    RGBColor ambient(ps.retrieve<RGBColor>("ambient", {0, 0, 0}), color_type);
    RGBColor diffuse(ps.retrieve<RGBColor>("diffuse", {0, 0, 0}), color_type);
    RGBColor specular(ps.retrieve<RGBColor>("specular", {0, 0, 0}), color_type);
    RGBColor mirror(ps.retrieve<Vec3>("mirror", {0, 0, 0}), color_type);
    auto glossiness = ps.retrieve<double>("glossiness", 0.0f);
    
    m_render_options->current_material = std::make_shared<BlinnPhongMaterial>(
        diffuse, specular, ambient, glossiness, mirror);
  }
  else if (type == "toon"){
    auto color_map = ps.retrieve<std::vector<double>>("color_map", {});
      auto color_type = ps.retrieve<std::string>("color_type", "rgb");
      std::vector<RGBColor> colors;
      RGBColor mirror(ps.retrieve<Vec3>("mirror", {0, 0, 0}), color_type);

      for(size_t i{0}; i + 2 < color_map.size(); i += 3){
        colors.push_back(RGBColor(color_map[i], color_map[i + 1], color_map[i + 2], color_type));
      }

      m_render_options->current_material = std::make_shared<ToonMaterial>(colors, mirror);
  }
}

void API::object(const ParamSet &ps) {
  check_in_world_block_state("API::object");
  auto type = ps.retrieve<std::string>("type", "unknown");
  if (type == "unknown") {
    ERROR("API::object(): Missing \"type\" specification for the object.");
  }
  if (type == "sphere") {
    auto radius = ps.retrieve<double>("radius", 0.0f);
    if (radius == 0.0f) {
      ERROR("API::object(): Missing \"radius\" specification for the sphere.");
    }
    auto center = ps.retrieve<Point3>("center", {0, 0, 0});
    m_render_options->elements.push_back(std::make_shared<Sphere>(
        center, radius, m_render_options->current_material));
  } else if (type == "triangle") {
    Point3 p0 = ps.retrieve<Point3>("p0", Point3(-1, 0, 0));
    Point3 p1 = ps.retrieve<Point3>("p1", Point3(1, 0, 0));
    Point3 p2 = ps.retrieve<Point3>("p2", Point3(0, 1, 0));

    m_render_options->elements.push_back(std::make_shared<Triangle>(
        p0, p1, p2, m_render_options->current_material));
  } else if (type == "plane") {
    Point3 p = ps.retrieve<Point3>("point", Point3(0, 0, 0));
    Vec3 n = ps.retrieve<Vec3>("normal", Vec3(0, 1, 0));

    m_render_options->elements.push_back(
        std::make_shared<Plane>(p, n, m_render_options->current_material));
  } else if (type == "star") {
    Point3 center = ps.retrieve<Point3>("center", Point3(0, 0, 0));
    double r_outer = ps.retrieve<double>("r_outer", 2.0);
    double r_inner = ps.retrieve<double>("r_inner", 0.8);

    m_render_options->elements.push_back(std::make_shared<Star>(
        center, r_outer, r_inner, m_render_options->current_material));
  } else if (type == "square") {
    Point3 p0 = ps.retrieve<Point3>("p0", Point3(-1, 0, -1));
    Point3 p1 = ps.retrieve<Point3>("p1", Point3(1, 0, -1));
    Point3 p2 = ps.retrieve<Point3>("p2", Point3(1, 0, 1));
    Point3 p3 = ps.retrieve<Point3>("p3", Point3(-1, 0, 1));

    m_render_options->elements.push_back(std::make_shared<Square>(
        p0, p1, p2, p3, m_render_options->current_material));
  } else if (type == "cube") {
    Point3 center = ps.retrieve<Point3>("center", Point3(0, 0, 0));
    double size = ps.retrieve<double>("size", 1.0);

    m_render_options->elements.push_back(std::make_shared<Cube>(
        center, size, m_render_options->current_material));
  } else
    ERROR("API::object(): Missing \"type\" specification for the object.");
}

void API::look_at(const ParamSet &ps) {
  check_in_setup_block_state("API::look_at");
  m_render_options->setup_params["lookat"] = ps;
}
void API::make_named_material(const ParamSet &ps) {
  check_in_world_block_state("API::make_named_material");
  auto type = ps.retrieve<string>("type", "unknown");
  auto color_type = ps.retrieve<std::string>("color_type", "spectre");

  auto mirror = RGBColor(ps.retrieve<Vec3>("mirror", Vec3()), color_type);
  if (type == "unknown") {
    ERROR("API::make_named_material(): Missing \"type\" specification for the "
          "material.");
  }
  auto material_id = ps.retrieve<std::string>("name", "unknown");
  if (material_id == "unknown") {
    ERROR("API::make_named_material(): Missing \"name\" specification for the "
          "material.");
  }

  if (type == "flat") {

    auto color_type = ps.retrieve<std::string>("color_type", "rgb");
    auto color =
        RGBColor(ps.retrieve<RGBColor>("color", RGBColor()), color_type);
    m_render_options->material_memory[material_id] =
        (std::shared_ptr<Material>(new FlatMaterial(color)));
  } else if (type == "blinn") {
    auto color_type = ps.retrieve<std::string>("color_type", "spectre");
    auto ambient =
        RGBColor(ps.retrieve<Vec3>("ambient", {0, 0, 0}), color_type);
    auto diffuse =
        RGBColor(ps.retrieve<Vec3>("diffuse", {0, 0, 0}), color_type);
    auto specular =
        RGBColor(ps.retrieve<Vec3>("specular", {0, 0, 0}), color_type);
    auto glossiness = ps.retrieve<double>("glossiness", 0.0f);
    
    m_render_options->material_memory[material_id] =
        std::make_shared<BlinnPhongMaterial>(diffuse, specular, ambient,
                                             glossiness, mirror);
  }
  else if (type == "toon"){
      auto color_map = ps.retrieve<std::vector<double>>("color_map", {});
      auto color_type = ps.retrieve<std::string>("color_type", "rgb");
      std::vector<RGBColor> colors;

      for(size_t i{0}; i + 2 < color_map.size(); i += 3){
        colors.push_back(RGBColor(color_map[i], color_map[i + 1], color_map[i + 2], color_type));
      }

      m_render_options->material_memory[material_id] = std::make_shared<ToonMaterial>(colors, mirror);
    }
}

void API::named_material(const ParamSet &ps) {
  auto material_id = ps.retrieve<string>("name", "unknown");
  if (m_render_options->material_memory.count(material_id)) {
    m_render_options->current_material =
        m_render_options->material_memory[material_id];
  } else {
    ERROR("API::named_material(): Missing or Invalid \"name\" specification "
          "for the material.");
  }
}

void API::render() {
  // Perform objects initialization here.
  // -------------------------------------------------------------
  // The Film object holds the memory for the image.

  // m_render_options->integrator =
  // make_integrator(m_render_options->setup_params["integrator"]);

  // auto primitive_list = std::make_shared<PrimitiveList>();
  // for(auto& obj : m_render_options->elements){
  //   primitive_list->add(obj);
  // }

  // m_render_options->scene = std::make_unique<Scene>(primitive_list,
  // m_render_options->background, m_render_options->light_sources);

  if (m_render_options->integrator && m_render_options->scene) {
    m_render_options->integrator->render(*m_render_options->scene);
  }
}

std::unique_ptr<rt::Camera> API::make_camera(const ParamSet &camera,
                                             const ParamSet &look_at,
                                             Resolution width,
                                             Resolution height) {
  std::unique_ptr<Camera> cam{nullptr};

  auto camera_type = camera.retrieve<string>("type", "unknown");
  if (camera_type == "orthographic" || camera_type == "perspective") {
    cam = Camera::create_camera(camera, look_at, width, height);
  } else {
    WARNING(string{"Camera \""} + camera_type + string{"\" unknown."});
  }
  return cam;
}

} // namespace rt
