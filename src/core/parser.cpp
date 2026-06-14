#include <cstdint>
#include <stdexcept>
#include <string>

#include "common.hpp"
#include "tinyxml2/tinyxml2.h"
#include <CLI11/CLI11.hpp>


#include "api.hpp"
#include "parser.hpp"

void print(const vector<string>& v) {
  for (auto s : v) {
    cout << s << endl;
  }
}

struct ImageFormatValidator : public CLI::Validator {
  ImageFormatValidator() {
    name_ = "IMAGE";
    func_ = [](const string &str) {
      string lower = CLI::detail::to_lower(str);

      // if ends with .png or .ppm
      if (lower.size() < 4 || (lower.substr(lower.size() - 4) != ".png" &&
                               lower.substr(lower.size() - 4) != ".ppm")) {
        return string("File must have .png or .ppm extension");
      }

      return string{};
    };
  }
};

struct XMLValidator : public CLI::Validator {
  XMLValidator() {
    name_ = "XML";
    func_ = [](const string &str) {
      string lower = CLI::detail::to_lower(str);

      // if ends with .xml
      if (lower.size() < 4 || (lower.substr(lower.size() - 4) != ".xml")) {
        return string("File must have .xml extension");
      }

      return string{};
    };
  }
};

const static ImageFormatValidator ImageFormatValidator;
const static XMLValidator XMLValidator;

namespace rt {

void Parser::validate_arguments(int argc, char **argv,
                                RunningOptions &run_opt) {
  CLI::App opts;
  opts.set_help_flag("--help,-h", "Print this help text.");

  string input_scene;
  vector<u_int16_t> crop_window;
  bool quick = false;
  bool verbose = false;
  string outfile;

  opts.add_option("<input_scene_file>", input_scene, "")
      ->required()
      ->type_name("")
      ->check(CLI::ExistingFile)
      ->check(XMLValidator);

  auto crop_opt = opts.add_option("--window,-w", crop_window,
                                  "Specify an image crop window.")
                      ->check(CLI::Range(0, 65535))
                      ->expected(4)
                      ->type_name("x1 y1 x2 y2");
  if (*crop_opt) {
    if (crop_window.size() != 4) {
      throw std::runtime_error("Crop window must have 4 values");
    }

    auto x1 = crop_window[0];
    auto y1 = crop_window[1];
    auto x2 = crop_window[2];
    auto y2 = crop_window[3];

    if (x2 < x1 || y2 < y1) {
      throw std::runtime_error("Invalid window");
    }

    run_opt.crop_region = {Pixel{x1, y1}, Pixel{x2, y2}};
  }
  opts.add_option("--outfile,-o", outfile,
                  "Write the rendered image to <filename>.")
      ->type_name("<filename>")
      ->check(ImageFormatValidator);

  opts.add_flag("--quick,-q", quick,
                "Reduces quality parameters to render image quickly.");

  opts.add_flag("--verbose", verbose, "Prints some internal info.");
  try {
    opts.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    std::exit(opts.exit(e));
  }

  run_opt.scene = input_scene;
  run_opt.quick = quick;
  run_opt.verbose = verbose;
  run_opt.outfile = outfile;

  if (*crop_opt) {
    run_opt.crop_region = {Pixel{crop_window[0], crop_window[1]},
                           Pixel{crop_window[2], crop_window[3]}};
  } else {
    run_opt.crop_region.reset();
  }
  opts.get_formatter()->column_width(40);
}

// @author = Selan Santos

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

/// Generic convertion function.
/*!
 * This function receives a string,`attr_content`, that may contain one or more
 * instances of type `T`, and tries to convert this string into a list of actual
 * elements of type `T`.
 *
 * In case there is only one instance of T, that instance is stored in the
 * ParamSet object `ps`, passed as output parameter. If there are more than one
 * instance of T, the function extracts them into a `std::vector<T>` and stores
 * the vector in `ps`.
 *
 * Therefore, the output of this function is the ParamSet object `ps` that
 * contains either a single instance of T or a vector<T> with multiples
 * instances of T, found in `attr_content`.
 *
 * Example: convert("h_res", "1920", ps), here the attribute "h_res" should
 * become an integer. The function inserts ps->m_map["h_res"]=1920 (`int`), in
 * the ParamSet object `ps`.
 *
 * @param attr_name The attribute name that will be associated with the value in
 * the ParamSet.
 * @param attr_content The string attribute value (or values) we wish to
 * convert.
 * @param[out] ps The output ParamSet object (dictionary).
 *
 * @return true if convertion worked, false otherwise.
 */

template <typename T>
bool convert(const string &attr_name, const string &attr_content,
             ParamSet *ps) {
  // assert(ps);
  std::istringstream iss{
      attr_content}; // Make `attr_content` a stream to extract data from.
  T single_value{};  // Stores a single value of type T...
  vector<T> multiple_values; // ... or use this to try to store multiple values.
  bool input_string_still_has_values{
      true}; // Assume we have values to read from.
  // [1]: Try to read several T-values from the input string `attr_content`.
  while (input_string_still_has_values and not iss.eof()) {
    // Slightly different treatment if T is bool
    if constexpr (std::is_same_v<T, bool>) {
      iss >> std::boolalpha >>
          single_value; // Use std::boolalpha to read "true" ou "false"
    } else {
      iss >> single_value; // Regular extraction.
    }
    // Failed while trying to extract value this time?
    if (iss.fail()) {
      if (multiple_values.empty()) { // Is it completely empty?
        return false;                // Failed! The input was empty all along.
      }
      // If we got here, at least one value was successfully extracted from
      // `attr_content`
      input_string_still_has_values = false;
      break;
    }
    std::cout << "   ----> Value extracted is " << single_value << '\n';
    // Store the single value into a vector & look for more.
    multiple_values.push_back(single_value);
  }
  // [2]: If we found only one value in the vector, we get rid of the vector and
  // store this single value in the ParamSet. Otherwise, we store the entire
  // vector.
  if (multiple_values.size() == 1) {
    single_value = multiple_values[0];
    ps->assign(attr_name, single_value);
  } else {
    ps->assign(attr_name, multiple_values);
  }
  return true;
}

/*!
 * This function will extract all the instances of **composite elements**
 * present in the input string `attr_content`. A composite element is a
 * homogeneous n-tuple of 2, 3 or 4 values of the same type, such as Point3f,
 * Vector2i, or Normal3f, for instance.
 *
 * @note
 * The composite type T **must have** the operator[]() implemented for this
 * function to work.
 *
 * @note
 * If the function finds only one instance of the composite element, then we
 * store a single value directly in the ParamSet, rather then a vector with just
 * one instance.
 *
 * @tparam T The basic type of the composit element.
 * @tparam N How many individual values each composite element has.
 * @param attr_name The attribute name.
 * @param attr_content The string attribute value we wish to convert.
 * @param[out] ps The output gc::ParamSet object.
 * @return A vector with all the composite elements extracted or no-value if
 * none is available.
 *
 * Example: A `Vector3f` has 3 elements of type `float`.
 * bl="255 255 51" will be stored as ps->m_map["bl"]=Color24(255,255,51) in the
 * `ParamSet` `ps`.
 */
template <typename T, std::uint16_t N>
bool convert(const string &attr_name, const string &attr_content,
             ParamSet *ps) {
  // assert(ps);
  std::istringstream iss{attr_content};
  vector<T> multiple_composite_values;
  T single_composite_value{};
  bool input_string_still_has_values{true};
  // [1] Keep reading groups of N values from the input string.
  while (input_string_still_has_values and not iss.eof()) {
    // Try to extract a N-tuple from the string.
    for (std::uint16_t idx{0}; idx < N; ++idx) {
      iss >> single_composite_value[idx]; // Try to extract a value.
      // Failed while extracting value this time around?
      if (iss.fail()) {
        if (multiple_composite_values.empty()) { // Completely empty?
          return false; // then, there is nothing to return.
        }
        input_string_still_has_values = false;
        break; // There is something in the vector.
      }
    }
    // Add the newly extracted composite item to the result vector.
    multiple_composite_values.push_back(single_composite_value);
  }
  // [2] If we found only one value in the vector, we get rid of the vector and
  // store this single value in the ParamSet. Otherwise, we store the entire
  // vector.
  if (multiple_composite_values.size() == 1) {
    single_composite_value = multiple_composite_values[0];
    ps->assign(attr_name, single_composite_value);
  } else {
    ps->assign(attr_name, multiple_composite_values);
  }
  return true;
}

/// This is the list of all supported tags and their corresponding
/// attributes/type.
std::unordered_map<string, vector<string>> tag_catalog{
    {
        "background",
        {
            "type",
            "filename",
            "mapping",
            "color_type",
            "color",
            "tl",
            "tr",
            "bl",
            "br",
        },
    },
    {
        "film",
        {
            "type",
            "filename",
            "img_type",
            "x_res",
            "y_res",
            "w_res",
            "h_res",
            "crop_window",
            "gamma_corrected",
        },
    },
    {"camera",
     {
         "type",
         "screen_window",
         "fovy",
     }},
    {"lookat",
     {
         "look_from",
         "look_at",
         "up",
     }},
     {"material",
      {
        "type",
        "color_type",
        "color",
        "ambient",
        "diffuse",
        "specular",
        "glossiness",
        "color_map",
        "mirror",
      }
     },
     {"object",
      {
        "type",
                      /// Sphere
        "radius",
        "center",
                      /// Plane
        "point",
        "normal",
                      /// TriangleMesh
        "indices",
        "ntriangles",
        "vertices",
        "vertex_indices",
        "normals",
        "normal_indices",
        "uvs",
        "uv_indices",
        "reverse_vertex_order",
        "compute_normals",
        "backface_cull",
        "filename",
      }
    },
    {
      "aggregator", 
      {
        "type",
		"max_prims_per_node",
      }
    },
    {
      "integrator",
      {
        "type",
        "depth",
        "blinn_phong",
        "mapping_interval",
        "n_intervals",
      }
    },
    {
      "make_named_material",
      {
        "type",
        "name",
        "color_type",
        "color",
        "ambient",
        "diffuse",
        "specular",
        "glossiness",
        "color_map",
        "mirror",
      }
    },
    {
      "named_material",
      {
        "name",
      }
    },
    {
      "light_source",
      {
        "type",
        "i",
        "scale",
        "from",
        "to",
        "attenuation",
        "cutoff",
        "falloff",
        "world_radius",
      }
    },
    {
      "include",
      {
        "filename",
      }
    },
    {
      "render_again",
      {""},   // no attributes
    },
    {
        "world_begin",
        {""}, // no attributes
    },
    {
        "world_end",
        {""}, // no attributes
    },
};

/// Maps the tag name to its corresponding API function.
std::unordered_map<string, std::function<void(const ParamSet &)>> api_functions{

    {"background", API::background}, {"camera", API::camera},
    {"lookat", API::look_at},        {"world_begin", API::world_begin},
    {"world_end", API::world_end},   {"film", API::film},
    {"material", API::material},     {"object", API::object},
    {"integrator", API::integrator}, {"make_named_material", API::make_named_material},
    {"named_material", API::named_material}, {"light_source", API::light_source},
    {"aggregator", API::aggregator},
};

/// Maps convertion function to an attribute name.
std::unordered_map<string, ConverterFunction> converters{
    {"type", convert<string>}, // "type" must be a string.
    {"name", convert<string>}, // "name" must be a string.
    //
    {"color_type", convert<string>},
    {"color", convert<RGBColor>}, // "color" is a Color24 with 3 fields.
    {"flip", convert<bool>},
    // Background attributes.
    {"mapping", convert<string>},
    {"bl", convert<RGBColor>},
    {"tl", convert<RGBColor>},
    {"tr", convert<RGBColor>},
    {"br", convert<RGBColor>},
    // Camera attributes
    // TODO: add aspect ratio
    {"fovy", convert<double>},
    {"screen_window", convert<Point4>},
    // Camera attributes but at lookat tag
    {"look_from", convert<Point3>},
    {"look_at", convert<Point3>},
    {"up", convert<Point3>},
    // Film attributes
    // TODO: crop_window
    {"x_res", convert<int>},
    {"y_res", convert<int>},
    {"w_res", convert<int>},
    {"h_res", convert<int>},
    {"filename", convert<string>},
    {"img_type", convert<string>},
    {"gamma_corrected", convert<bool>},
    // Material attributes
    {"color_map", convert<double>},
    {"mirror", convert<Vec3>},
    // Object attributes
    {"radius", convert<double>},
    {"center", convert<Point3>},
    
    {"point", convert<Point3>},
    {"normal", convert<Vec3>},

    {"indices", convert<int>},
    {"ntriangles", convert<int>},
    {"vertices", convert<Point3>},
    {"vertex_indices", convert<int>},
    {"normals", convert<Vec3>},
    {"normal_indices", convert<int>},
    {"uvs", convert<Point2>},
    {"uv_indices", convert<int>},
    {"reverse_vertex_order", convert<string>},
    {"compute_normals", convert<string>},
    {"backface_cull", convert<string>},
	  // Integrator
	  {"zmin", convert<double>},
	  {"zmax", convert<double>},
    {"near_color", convert<RGBColor>},
    {"far_color", convert<RGBColor>},
    
    {"mapping_interval", convert<double>},
    {"n_intervals", convert<int>},
    {"depth", convert<int>},
    // Light attributes
    {"i", convert<RGBColor>},
    {"scale", convert<RGBColor>},
    {"from", convert<Point3>},
    {"to", convert<Vec3>},
    {"attenuation", convert<Vec3>},
    {"ambient", convert<Vec3>},
    {"diffuse", convert<Vec3>},
    {"specular", convert<Vec3>},
    {"glossiness", convert<double>},
    {"depth", convert<double>},
    {"cutoff", convert<double>},
    {"falloff", convert<double>},
    {"world_radius", convert<double>},
    {"max_prims_per_node", convert<int>},
};

/*!
 * This function checks if the tag received is valid.
 * @param tag_name The tag name we want to validate.
 */
bool is_valid_tag(std::string_view tag_name) {
  // Check if we have a valid registered tag name.
  auto tag_query{tag_catalog.find((string)tag_name)};
  return tag_query != tag_catalog.end();
}

/*!
 * This function checks if the attribute name belongs to a given tag name.
 * @note The precondition is that tag_name is valid.
 * @param tag_name A valid tag name.
 * @attribute_name The attribute name we want to validate.
 */
bool is_valid_attribute(std::string_view tag_name,
                        std::string_view attribute_name) {
  // Get the attribute list associated with `tag_name`.
  auto attribute_list{tag_catalog[(string)tag_name]};
  auto attr_query =
      std::find(attribute_list.begin(), attribute_list.end(), attribute_name);
  return attr_query != attribute_list.end();
}

/*!
 * This function invokes a converter function that translates the attribute
 * content (as a string) into the expected type and store it into the
 * gc::ParamSet object received as input argument.
 * @param attr_name The attribute name.
 * @param attr_content The attribute value as a string.
 * @param ps A reference to the current gc::ParamSet object we are filling in.
 */
void parse_attribute(const string &attr_name /* IN value */,
                     const string &attr_content /* IN value */,
                     ParamSet *ps /* OUT value*/) {
  std::ostringstream oss;
  // Find the proper convertion function.
  auto converter_func = converters[attr_name];
  if (converter_func) { // Do we have one defined?
    if (converter_func(attr_name, attr_content, ps)) {
      oss << " ⁺ Successfuly converted attribute " << std::quoted(attr_name);
      MESSAGE(oss.str());
    } else {
      oss << " - Convertion of " << std::quoted(attr_name) << " failed!";
      MESSAGE(oss.str());
    }
  } else {
    oss << " - Could not find a convertion function for the tag "
        << std::quoted(attr_name) << ". Skipping...";
    WARNING(oss.str());
  }
}

/*!
 * This is the entry point where the parsing of the scene file begins.
 */
void Parser::parse_scene(const string& filename) {
  // [1] Load document.
  tinyxml2::XMLDocument doc;
  if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
    std::cerr << "Error loading the XML file!" << '\n';
    return;
  }

  // [2] Get the Root node
  tinyxml2::XMLElement *root = doc.RootElement();
  if (root == nullptr) {
    std::cerr << "Root node of the XML tree was not found!" << '\n';
    return;
  }

  // [3] Iterate over every child elements, i.e. over every tag.
  for (tinyxml2::XMLElement *child_node = root->FirstChildElement();
       child_node != nullptr; child_node = child_node->NextSiblingElement()) {
    // ================================================================================
    // Validate the current tag name.
    // --------------------------------------------------------------------------------
    string tag_name = str_to_lower(child_node->Name());
    if (not is_valid_tag(tag_name)) {
      std::ostringstream oss;
      oss << "The tag " << std::quoted(tag_name) << " is not valid!";
      WARNING(oss.str());
      continue; // Skip to the next tag in the scene file.
    }
    {
      std::ostringstream oss;
      oss << ">>>>> Started parsing tag " << std::quoted(tag_name) << ".";
      MESSAGE(oss.str());
    }
    // ================================================================================
    // At this point we have a valid tag name. Now we need to validate its
    // attributes.
    // --------------------------------------------------------------------------------
    // Create the empty gc::ParamSet object to store the attributes we will
    // process next.
    ParamSet ps;
    // Iterate over this tag's attributes
    for (const tinyxml2::XMLAttribute *attr = child_node->FirstAttribute();
         attr != nullptr; attr = attr->Next()) {
      // Validate the current attribute name.
      string attribute_name{str_to_lower(attr->Name())};
      if (not is_valid_attribute(tag_name, attribute_name)) {
        std::ostringstream oss;
        oss << "The tag " << std::quoted(tag_name)
            << " does not have an attribute " << std::quoted(attribute_name)
            << ". Ignoring...";
        WARNING(oss.str());
        continue; // Skip to the next attribute inside this tag.
      }
      // Parse the string version of this attribute into its expected value.
      // The result is stored inside the gc::ParamSet object, passed in as the
      // last argument.
      string attribute_value{str_to_lower(attr->Value())};
      parse_attribute(attribute_name, attribute_value, /*OUT value*/ &ps);
    }
    // ================================================================================
    // Now we have gc::ParamSet object filled in and ready to be passed along to
    // the API.
    // ================================================================================
    // ============================================================================
    /// HACK: If the tag is `include` we call `parse_scene_file()` recursively.
    // ----------------------------------------------------------------------------
    if (tag_name == "include") {
      auto filename = ps.retrieve<string>("filename", "");
      if (filename.empty()) {
        WARNING("Missing attribute \"filename\" in tag \"include\"");
        continue;
      }
      if (not fs::exists(fs::path{filename.c_str()})) {
        std::ostringstream oss;
        oss << "Included file " << std::quoted(filename) << " does not exist.";
        ERROR(oss.str());
      }
      // Recursive call to process subfile.
      
      parse_scene(filename.c_str());
      continue; // This tag doesn't have an API function associated with; get
                // next tag.
    }
    if (tag_name == "render_again") {
      std::ostringstream oss;
      oss << "<<<<< <render_again> tag was founded! Starting the Rendering "
             "phase."
          << ".\n";
      MESSAGE(oss.str());
      rt::API::m_api_state = rt::API::World;
      rt::API::world_end(ps);
      continue;
    }
    // ============================================================================

    // Check whether this tag_name has a proper API function.
    if (api_functions.count(tag_name) == 0) {
      std::ostringstream oss;
      oss << "The tag " << std::quoted(tag_name)
          << " does not have a corresponding API function associated with. "
             "Ignoring...";
      WARNING(oss.str());
      continue;
    }

    {
      std::ostringstream oss;
      oss << "<<<<< Calling API function for the tag " << std::quoted(tag_name)
          << ".\n";
      MESSAGE(oss.str());
    }
    // Call the api function associated with the tag name.
    api_functions[tag_name](ps);
  }
}

} // namespace rt
