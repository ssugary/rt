#ifndef PARSER_HPP
#define PARSER_HPP

#include <algorithm>
#include <functional>

#include "common.hpp"
#include "paramset.hpp"

namespace rt {

// @author = Selan Santos
// ===
/// Lambda expression that transform a c-style string to a lowercase c++-stype
/// string version.
static auto str_to_lower = [](const char *c_str) -> string {
  string str{c_str};
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
};

/// Map that binds a convertion function to an attribute name.
using ConverterFunction =
    std::function<bool(const string &, const string &, ParamSet *)>;
// ===

class Parser {
public:
  ~Parser() = default;

  static void validate_arguments(int argc, char **argv, RunningOptions &run_opt);
  static void parse_scene(const string& filename);

};

} // namespace rt

bool is_valid_tag(std::string_view tag_name);
bool is_valid_attribute(std::string_view tag_name, std::string_view attribute_name);

void parse_attribute(const string &attr_name, const string &attr_content, rt::ParamSet *ps);

void print(const rt::RunningOptions& run_opt);

#endif //< PARSER_HPP