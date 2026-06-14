#include "background.hpp"
#include "camera.hpp"
#include "common.hpp"
#include "include_integrator.hpp"
#include <algorithm>

namespace rt {

std::optional<rt::RGBColor>
DepthMapIntegrator::Li(const Ray &ray, const rt::Scene &scene, const int& depth) const {
  rt::Surfel isect;

  if (!scene.intersect(ray, &isect)) {
    return std::nullopt;
  }

  Point3 ray_hit = isect.p;
  Point3 cam_origin = camera->get_origin();

  double hit_dist = euclidean_distance_3D(cam_origin, ray_hit);

  double z_norm = (hit_dist - m_zmin_abs) / (m_zmax_abs - m_zmin_abs);

  z_norm = std::clamp(z_norm, m_zmin, m_zmax);

  RGBColor near = m_near_color * m_zmin;
  RGBColor far = m_far_color * m_zmax;

  RGBColor color =
      Background::linear_interpolation(near, far, z_norm);

  return color;
}

void DepthMapIntegrator::preprocess(const rt::Scene &scene) {
  auto w = camera->film->width();
  auto h = camera->film->height();

  Surfel isect;
  // -------------------------------------------------------------
  // Traverse all pixels to shoot rays from.
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      Ray ray = camera->generate_ray(i, j);

      if (!scene.intersect(ray, &isect)) {
        continue;
      }

      Point3 ray_hit = isect.p;
      Point3 cam_origin = camera->get_origin();

      double hit_dist = euclidean_distance_3D(cam_origin, ray_hit);

      if (hit_dist < m_zmin_abs) {
        m_zmin_abs = hit_dist;
      }

      if (hit_dist > m_zmax_abs) {
        m_zmax_abs = hit_dist;
      }
    }
  }
};

void DepthMapIntegrator::render(const rt::Scene &scene) {
  // Perform objects initialization here.
  // -------------------------------------------------------------
  // The Film object holds the memory for the image.

  if (!camera) {
    return;
  }

  preprocess(scene);

  auto w = camera->film->width();
  auto h = camera->film->height();

  // -------------------------------------------------------------
  // Traverse all pixels to shoot rays from.
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      Ray ray = camera->generate_ray(i, j);
      // Determine the incoming light.
      auto temp_L = Li(ray, scene, 0);

      rt::RGBColor L = (temp_L.has_value()) ? temp_L.value() : m_far_color;

      camera->film->add(rt::Pixel{static_cast<byte>(i), static_cast<byte>(j)},
                        L);
    }
  }
  // send image color buffer to the output file.
  camera->film->write_image();
  cout << m_zmin_abs << endl;
  cout << m_zmax_abs << endl;
}

} // namespace rt
