#ifndef _OMG_TR_MESH_
#define _OMG_TR_MESH_

#include "omg/common.h"

namespace omg {
/**
 * Represents a triangle mesh.
 *
 * @author Vitor Greati
 * */
struct TriangleMesh {

    const int n_triangles;
    const int n_vertices;
    std::vector<int> vertex_indices;

    std::unique_ptr<Vec3[]> points;
    std::unique_ptr<Vec3[]> normals;
    std::unique_ptr<Point2[]> uvs;

    TriangleMesh(int n_triangles, const int *indices, int n_vertices,
            const Point3 *ps, const Vec3* ns, const Point2* uvs) 
    : n_triangles {n_triangles}, n_vertices {n_vertices},
    vertex_indices {indices, indices + 3 * n_triangles } {
        
        points.reset(new Point3[n_vertices]);
        for (int i {0}; i < n_vertices; ++i)
            points[i] = ps[i];

        normals.reset(new Vec3[n_triangles]);
        for (int i {0}; i < n_triangles; ++i)
            normals[i] = ns[i];

        this->uvs.reset(new Point2[n_triangles]);
        for (int i {0}; i < n_triangles; ++i)
            (this->uvs)[i] = uvs[i];

    };

};
};

#endif
