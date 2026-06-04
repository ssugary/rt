#include "sphere.hpp"

namespace rt{

bool Sphere::intersect(const Ray &r, float *t_hit, Surfel *sf) const {
  Point3 oc = r.getOrigin() - center;
  Vec3 v = r.getDirection();

  double A = dot(v, v);
  double hB = dot(oc, v);
  double C = dot(oc, oc) - radius * radius;

  double delta = hB * hB - A * C;

  if (delta < 0)
    return false;

  double sqr = std::sqrt(delta);
  
  double n = (hB > 0) ? -hB - sqr : -hB + sqr; // to avoid precision lost

  double t0 = n / A;
  double t1 = C / n;

  if (t0 > t1) {
	  std::swap(t0, t1);
  }

  if (t0 < r.getTMin() || t0 > r.getTMax()) {
      t0 = t1;
      if (t0 < r.getTMin() || t0 > r.getTMax()) {
          return false; 
      }
  }

  if(t_hit) *t_hit = t0;

  if (sf) {
    sf->time = t0;
    sf->p = r(t0);
    sf->n = (sf->p - center) / radius;

    if (flips_normal) sf->n = -sf->n;
    
    sf->wo = -r.getDirection();

  }

  return true;
}

bool Sphere::intersect_p(const Ray &r) const {
  Point3 oc = r.getOrigin() - center;
  Vec3 v = r.getDirection();

  double A = dot(v, v);
  double hB = dot(oc, v);
  double C = dot(oc, oc) - radius * radius;

  double delta = hB * hB - A * C;

  if (delta < 0)
    return false;

  double sqr = std::sqrt(delta);

  double n = (hB > 0) ? -hB - sqr : -hB + sqr; // to avoid precision lost

  double t0 = n / A;
  double t1 = C / n;

  if (t0 > t1) {
	  std::swap(t0, t1);
  }

  if (t0 >= r.getTMin() && t0 <= r.getTMax())
    return true;

  if (t1 >= r.getTMin() && t1 <= r.getTMax())
    return true;

  return false;
}


}