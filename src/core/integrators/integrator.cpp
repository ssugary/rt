#include "integrator.hpp"


namespace rt{
void SamplerIntegrator::render(const rt::Scene& scene){
      // Perform objects initialization here.
  // -------------------------------------------------------------
  // The Film object holds the memory for the image.

  if(!camera){
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
        auto temp_L =  Li( ray, scene );

        auto u = float(i) / float(w);
        auto v = 1.0f - float(j) / float(h);
        rt::RGBColor L = (temp_L.has_value()) ?  temp_L.value() : scene.background->sample(u, v);

        L.red = std::clamp(L.red, 0.0, 1.0);
        L.green = std::clamp(L.green, 0.0, 1.0);
        L.blue = std::clamp(L.blue, 0.0, 1.0);

        camera->film->add(rt::Pixel{static_cast<u_int64_t>(i), static_cast<u_int64_t>(j)}, L);
      }
  }
    // send image color buffer to the output file.
    camera->film->write_image();    
}
}