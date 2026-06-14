#ifndef RAY_HPP
#define RAY_HPP

#include <cmath>

#include "ssmath/ssmath.hpp"

using Point3 = vec3<double>;
using Vector3 = vec3<double>;

class Ray {
private:
  Point3 origin{};
  Vector3 direction{};

  mutable double t_min{}, t_max{};

public:
  Ray() : t_min(0.0), t_max(INFINITY) {};

  Ray(const Point3& o, const Vector3& dir, double t_min = 0.0, double t_max = INFINITY)
      : origin(o), direction(dir), t_min(t_min), t_max(t_max) {};

  void normalize() { direction.mk_unit_vec(); };

  void setOrigin(const Point3& o) { this->origin = o; };
  void setDirection(const Vector3& dir) { this->direction = dir; };
  void setTmin(const double& t_min) const { this->t_min = t_min; }
  void setTMax(const double& t_max) const { this->t_max = t_max; }

  Point3 getOrigin() const { return this->origin; };
  Vector3 getDirection() const { return this->direction; };
  double getTMin() const { return this->t_min; }
  double getTMax() const { return this->t_max; }

  constexpr Point3 operator()(const double& t) const { return origin + direction * t; }

  friend std::ostream &operator<<(std::ostream &os, const Ray &ray);
};

inline std::ostream &operator<<(std::ostream &os, const Ray &ray) {
  return os << "[o=(" << ray.origin.x() << "," << ray.origin.y() << ","
            << ray.origin.z() << "), d=(" << ray.direction.x() << ","
            << ray.direction.y() << "," << ray.direction.z()
            << "), tmin: " << ray.t_min << ", tmax: " << ray.t_max << "]";
}
#endif
