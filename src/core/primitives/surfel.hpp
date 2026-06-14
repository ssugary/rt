#ifndef SURFEL_HPP
#define SURFEL_HPP

#include <ssmath/ssmath.hpp>

#ifndef PRIMITIVE_HPP
namespace rt{
struct Surfel;
}
#include "primitive.hpp"
#endif  //< PRIMITIVE_HPP

using Mat4 = mat4<double>;
using Point2 = vec2<double>;
using Point3 = vec3<double>;
using Point4 = vec4<double>;
using Vec3 = vec3<double>;

namespace rt{
struct Surfel {
public:
  Surfel(const Point3 &p, const Vec3 &n, const Vec3 &wo, float time,
         const Point2 &uv, const Primitive *pri)
      : p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri} { /* empty */ };

  Surfel() = default;

  Point3 p; //!< Contact point.
  Vec3 n;   //!< The surface normal.
  Vec3 wo;  //!< Outgoing direction of light, which is -ray.
  double time;
  Point2 uv; //!< Parametric coordinate (u,v) of the hit surface.
  const Primitive *primitive = nullptr; //!< Pointer to the primitive.
};
}
#endif