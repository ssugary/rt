#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>

#include "ssmath/ssmath.hpp"

#include "film.hpp"
#include "ray.hpp"

using Mat4 = mat4<double>;
using Point3 = vec3<double>;
using Point4 = vec4<double>;
using Vec3 = vec3<double>;

namespace rt {

/**
 * @class Camera
 * @brief Manages the projection of the 3D scene onto the 2D image plane.
 * @details This class calculates and stores the transformation matrices
 * (Camera-to-World and Raster-to-Screen) to generate rays in an optimized
 * manner during rendering. It natively supports oblique projections by
 * parameterizing the View Plane Normal (VPN).
 */
class Camera {
public:
  /**
   * @brief Destructor
   */
  virtual ~Camera() = default;

  std::unique_ptr<Film> film; //< The film object that will store the image.

  /**
   * @brief Generates a ray starting from the camera and passing through the
   * specified pixel.
   * @param i The x-coordinate (column) of the pixel in the raster.
   * @param j The y-coordinate (row) of the pixel in the raster.
   * @return Ray The mathematical ray configured with its origin and direction
   * in world space.
   */
  virtual Ray generate_ray(const int& i, const int& j) = 0;
  Point3 get_origin() { return m_origin;};  
  /**
   * @brief Function that will retrieve the data and create a camera.
   * @param camera The ParamSet of the camera tag.
   * @param look_at The ParamSet of the lookat tag.
   * @return The pointer to the camera that will be owned by the RenderOptions.
   */
  static std::unique_ptr<Camera> create_camera(const ParamSet &camera,
                                        const ParamSet &look_at,
										const Resolution x, const Resolution y);

protected:
  Mat4 m_camera_to_world;  //< Matrix that transforms from camera space to world
                           // space.
  Mat4 m_raster_to_screen; //< Matrix that transforms from pixel coordinates to
                           // the camera screen.

  Point3 m_origin;         //< Camera position.
  Vec3 m_gaze_dir;         //< Gaze direction.
  double m_focal_distance; //< Focal distance used in perspective projection.
};

class OrthographicCamera : public Camera {
public:
  /**
   * @brief OrthographicCamera constructor. Pre-computes all transformation
   * matrices.
   * @param look_from The camera's position in world space.
   * @param look_at The target point in space the camera is aiming at.
   * @param vup The View-Up vector (defines which direction is "up").
   * @param l Left boundary of the projection window.
   * @param r Right boundary of the projection window.
   * @param b Bottom boundary of the projection window.
   * @param t Top boundary of the projection window.
   * @param nx Horizontal resolution of the final image in pixels.
   * @param ny Vertical resolution of the final image in pixels.
   * @param focal_dist Focal distance (distance from the camera to the
   * projection plane). Relevant for perspective.
   */
  OrthographicCamera(Point3 look_from, const Point3 look_at, const Vec3 vup,
                     double l, double r, double b, double t, int nx, int ny,
                     double focal_dist = 1.0);

  Ray generate_ray(const int& i, const int& j) override;
};

class PerspectiveCamera : public Camera {
public:
  /**
   * @brief Camera constructor. Pre-computes all transformation matrices.
   * @param type The projection type (ORTHOGRAPHIC or PERSPECTIVE).
   * @param look_from The camera's position in world space.
   * @param look_at The target point in space the camera is aiming at.
   * @param vup The View-Up vector (defines which direction is "up").
   * @param vpn The View Plane Normal vector (defines the tilt of the projection
   * plane, useful for oblique projections).
   * @param l Left boundary of the projection window.
   * @param r Right boundary of the projection window.
   * @param b Bottom boundary of the projection window.
   * @param t Top boundary of the projection window.
   * @param nx Horizontal resolution of the final image in pixels.
   * @param ny Vertical resolution of the final image in pixels.
   * @param focal_dist Focal distance (distance from the camera to the
   * projection plane). Relevant for perspective.
   */
  PerspectiveCamera(Point3 look_from, const Point3 look_at, const Vec3 vup,
                    Vec3 vpn, int nx, int ny, double fovy, double focal_dist = 1.0);

  Ray generate_ray(const int& i, const int& j) override;

private:
  double m_fovy;
};
} // namespace rt
#endif // CAMERA_HPP
