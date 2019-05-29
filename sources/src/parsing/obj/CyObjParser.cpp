#include "omg/parsing/obj/CyObjParser.h"
#include "omg/objects/Triangle.h"
#include "external/cycodebase/cyTriMesh.h"
#include "omg/exceptions.h"

using namespace omg;

std::vector<std::shared_ptr<Triangle>> CyObjParser::parse_tri_mesh(const std::string& file_name, bool bfc) const {

    cy::TriMesh cy_trimesh;

    if(!cy_trimesh.LoadFromFileObj(file_name.c_str()))
        throw omg::ParseException("fail at loading obj file in " + file_name);

    auto nvertices = cy_trimesh.NV();
    auto ntriangles = cy_trimesh.NF();

    //> Process indices
    std::vector<int> indices;
    for (int i = 0; i < ntriangles; ++i) {
        auto tri_face = cy_trimesh.F(i);
        for (auto j {0}; j < 3; ++j) {
            indices.push_back(tri_face.v[j]);
        }
    }

    //> Process vertices
    std::vector<Point3> vertices;
    for (int i = 0; i < nvertices; ++i) {
        const auto& [x, y, z] = cy_trimesh.V(i);
        vertices.push_back(Point3{x, y, z});
    }

    //> Process normals
    std::vector<Vec3> normals;
    auto nnormals = cy_trimesh.NVN();
    for (int i = 0; i < nnormals; ++i) {
        const auto& [x, y, z] = cy_trimesh.VN(i);
        normals.push_back(Vec3{x, y, z}); 
    }
    auto last = normals.back();
    while (nnormals < nvertices) {
        normals.push_back(last);
        nnormals++;
    }

    //> Process barycentric
    std::vector<Point2> uvs;
    //TODO
    
    return Triangle::create_triangle_mesh(bfc, ntriangles, indices.data(),
            nvertices, vertices.data(), normals.data(), uvs.data());
}
