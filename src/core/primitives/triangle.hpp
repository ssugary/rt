#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "tinyobj/tiny_obj_loader.h"
#include "paramset.hpp"
#include "primitive.hpp"
#include "shape.hpp"
#include <memory>
#include <ostream>
#include "chrono"
namespace rt {

    struct TriangleMesh {

        int n_triangles;
  
        vector<int> vertex_indices;   
        vector<int> normal_indices;   
                                        
        vector<int> uvcoord_indices;

        vector<Point3>  vertices;
        vector<Vec3>    normals;
        vector<Point2> uvcoords;

        TriangleMesh() = default;
        TriangleMesh(const TriangleMesh&) = delete;
        TriangleMesh& operator=(const TriangleMesh&) = delete;
        TriangleMesh(TriangleMesh&& other) = delete;


    };

    class Triangle : public Shape {
        private:
            int* v;
            int* n;
            int* uv;
            bool backface_cull;

            std::shared_ptr<TriangleMesh> mesh;
        public:
            
            Triangle(bool flip, const std::shared_ptr<TriangleMesh>& mesh, int tri_id, bool bfc = true)
                :Shape(flip), backface_cull{ bfc }, mesh{ mesh } {
     
                v = &mesh->vertex_indices[3 * tri_id];
                n = &mesh->normal_indices[3 * tri_id];
                uv = &mesh->uvcoord_indices[3 * tri_id];
            }

            bool intersect(const Ray &r, float* t_hit, Surfel *sf) const override;
            bool intersect_p(const Ray &r) const override;
            bool box(Bounds3f &box) const override;
            friend std::ostream& operator<<(std::ostream& os, const Triangle& t);
    };

    std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape(bool flip_normals, const ParamSet& ps);
    std::vector<std::shared_ptr<Shape>> create_triangle_mesh(const std::shared_ptr<TriangleMesh>&, const bool&, const bool&);

    bool load_mesh_data(const std::string& filename,
                    bool rvo,
                    bool cn,
                    bool fn,
                    const std::shared_ptr<TriangleMesh>& md);

    void extract_obj_data(const tinyobj::attrib_t& attrib,
                        const std::vector<tinyobj::shape_t>& shapes,
                        bool reverse_order,
                        bool cn,
                        bool fn,
                        const std::shared_ptr<TriangleMesh>& md);


    };

#endif
