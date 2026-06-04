#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "paramset.hpp"
#include "primitive.hpp"
#include "tinyobj/tiny_obj_loader.h"
#include <memory>
#include <ostream>

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

    class Triangle : public Primitive{
        private:
            int* v;
            int* n;
            int* uv;
            bool backface_cull;

            std::shared_ptr<TriangleMesh> mesh;
        public:
            
            Triangle(std::shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc = true)
                : backface_cull{ bfc }, mesh{ mesh } {
     
                v = &mesh->vertex_indices[3 * tri_id];
                n = &mesh->normal_indices[3 * tri_id];
                uv = &mesh->uvcoord_indices[3 * tri_id];
            }



            bool intersect(const Ray &r, Surfel *sf) const override;
            bool intersect_p(const Ray &r) const override;
            // Bounds3f object_bound() const;
            friend std::ostream& operator<<(std::ostream& os, const Triangle& t);
    };

    std::vector<std::shared_ptr<Primitive /*Shape*/>> create_triangle_mesh_shape(bool flip_normals, const ParamSet& ps);
    std::vector<std::shared_ptr<Primitive /*Shape*/>> create_triangle_mesh(std::shared_ptr<TriangleMesh>, bool);

    bool load_mesh_data(const std::string& filename,
                    bool rvo,
                    bool cn,
                    bool fn,
                    std::shared_ptr<TriangleMesh> md);

    void extract_obj_data(const tinyobj::attrib_t& attrib,
                        const std::vector<tinyobj::shape_t>& shapes,
                        bool reverse_order,
                        bool cn,
                        bool fn,
                        std::shared_ptr<TriangleMesh> md);


    };

#endif