#include "triangle.hpp"

namespace rt {
    
    Triangle::Triangle(Point3 p0, Point3 p1, Point3 p2, std::shared_ptr<Material> mat)
    : p0(p0), p1(p1), p2(p2) {
      this->material = mat;
    }

bool Triangle::intersect(const Ray &r, Surfel *sf) const {
  Vec3 o = r.getOrigin() - this->p0; //< Vector (o - v0)
  Vec3 d = r.getDirection();         //< Ray direction
  Vec3 v20 = this->p2 - this->p0;    //< Vector (v2 - v0)
  Vec3 v10 = this->p1 - this->p0;    //< Vector (v1 - v0)

  Vec3 C = cross(v20, d);

  double det = dot(v10, C);

  if(std::abs(det) < epsilon){
    return false;
  }
  
  double invdet = 1/det;

  double U = invdet * dot(C, o); //< U = det(O, v10, v20) / det(v20, v10, d)
  if(U < 0.0 || U > 1.0){   //< u ≥ 0 e v ≥ 0, e u + v ≤ 1
    return false;
  } 
  Vec3 vcross = cross(v10, o);
  double V = invdet * dot(vcross, d); //< V = det(-d, O, v20) / det(v20, v10, d)

  if(V < 0.0 || U + V > 1.0){ //< u ≥ 0 e v ≥ 0, e u + v ≤ 1
    return false;
  }

  double t = invdet * dot(v20, vcross); //< t = det(-d, v10, O) / det(v20, v10, d)
  if(t < r.getTMin() || t > r.getTMax()){      //< t in range
    return false;
  }

  if (sf) {
    sf->time = t;
    sf->p = r(t);
    sf->n = cross(v10, v20);
    sf->n.mk_unit_vec();

    if (dot(r.getDirection(), sf->n) > 0) {
        sf->n = -sf->n;
    }

    sf->wo = -r.getDirection();
    sf->uv = {U, V};
    sf->primitive = this;
  }
  r.setTMax(t);

  return true;
}

bool Triangle::intersect_p(const Ray &r) const {

  Vec3 o = r.getOrigin() - this->p0; //< Vector (o - v0)
  Vec3 d = r.getDirection();         //< Ray direction
  Vec3 v20 = this->p2 - this->p0;    //< Vector (v2 - v0)
  Vec3 v10 = this->p1 - this->p0;    //< Vector (v1 - v0)

  Vec3 C = cross(v20, d);

  double det = dot(v10, C);

  if(std::abs(det) < epsilon){
    return false;
  }
  
  double invdet = 1.0 /det;

  double U = invdet * dot(C, o); //< U = det(O, v10, v20) / det(v20, v10, d)
  if(U < 0.0 || U > 1.0){   //< u ≥ 0 e v ≥ 0, e u + v ≤ 1
    return false;
  } 
  Vec3 vcross = cross(v10, o);
  double V = invdet * dot(vcross, d); //< V = det(-d, O, v20) / det(v20, v10, d)

  if(V < 0.0 || U + V > 1.0){ //< u ≥ 0 e v ≥ 0, e u + v ≤ 1
    return false;
  }

  double t = invdet * dot(v20, vcross); //< t = det(-d, v10, O) / det(v20, v10, d)
  if(t < r.getTMin() || t > r.getTMax()){      //< t in range
    return false;
  }
  return true;
}

}