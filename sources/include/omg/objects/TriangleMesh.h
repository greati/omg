#ifndef _OMG_TR_MESH_
#define _OMG_TR_MESH_

#include "omg/common.h"
#include <iostream>

namespace omg {
/**
 * Represents a triangle mesh.
 *
 * @author Vitor Greati
 * */
struct TriangleMesh {

    bool compute_normals {false};

    const int n_triangles;
    const int n_vertices;
    std::vector<int> vertex_indices;

    std::unique_ptr<Vec3[]> points;
    std::unique_ptr<Vec3[]> normals;
    std::unique_ptr<Point2[]> uvs;

    TriangleMesh(
            const std::shared_ptr<Transform>& object_to_world,
            int n_triangles, const int *indices, int n_vertices,
            const Point3 *ps, const Vec3* ns, const Point2* uvs, bool compute_normals = false) 
    : n_triangles {n_triangles}, n_vertices {n_vertices},
    vertex_indices {indices, indices + 3 * n_triangles }, compute_normals {compute_normals} {
        
        points.reset(new Point3[n_vertices]);
        for (int i {0}; i < n_vertices; ++i) {
            points[i] = object_to_world->t_point3(ps[i]);
        }

        normals.reset(new Vec3[n_vertices]);
        for (int i {0}; i < n_vertices; ++i) {
            normals[i] = object_to_world->t_normal3(ns[i]);
        }

        //TODO
        //this->uvs.reset(new Point2[n_vertices]);
        //for (int i {0}; i < n_vertices; ++i)
        //    (this->uvs)[i] = uvs[i];
        //

    };

};
};

/*std::ostream& operator<<(std::ostream& out, const omg::TriangleMesh& mat) {
    out << "NT: " << mat.n_triangles << std::endl;
    out << "NV: " << mat.n_vertices << std::endl;
    for (const auto& i : mat.vertex_indices)
        out << i;
    return out;
}*/

#endif
