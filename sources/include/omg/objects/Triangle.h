#ifndef _OMG_TRIANGLE_
#define _OMG_TRIANGLE_

#define EPSILON 0.000001

#include "omg/objects/Object.h"
#include "TriangleMesh.h"
#include <cmath>

namespace omg {
/**
 * Represents a triangle in a mesh.
 *
 * @author Vitor Greati
 * */
class Triangle : public Object {

    private:

        std::shared_ptr<TriangleMesh> mesh;
        const int *v;
        bool bfc {true};
        bool clockwise {false};

    public:

        Triangle(bool bfc, const std::shared_ptr<TriangleMesh>& mesh, int tri_number, bool clockwise = false) 
            : bfc {bfc}, mesh {mesh}, clockwise {clockwise} {
            v = &mesh->vertex_indices[3 * tri_number]; 
        }

        static std::vector<std::shared_ptr<Triangle>> create_triangle_mesh(
                bool bfc, int n_triangles, const int *indices, int n_vertices,
                const Point3 *ps, const Vec3* ns, const Point2* uvs, bool compute_normals = false,
                bool clockwise = false) {
            std::shared_ptr<TriangleMesh> mesh = 
                std::make_shared<TriangleMesh>(n_triangles, indices, n_vertices,
                        ps, ns, uvs, compute_normals);
            std::vector<std::shared_ptr<Triangle>> triangles;
            for (int i = 0; i < n_triangles; ++i)
                triangles.push_back(std::make_shared<Triangle>(bfc, mesh, i, clockwise));
            return triangles;
        } 

        bool intersect(const Ray& ray, float * tHit, SurfaceInteraction* hit_record) override {
            const auto& r_origin = ray.get_origin();
            auto r_direction = ray.get_direction();//tao::unitize(ray.get_direction());

            if (hit_record != nullptr) {
                r_direction = tao::unitize(r_direction);
            }

            auto [p0, p1, p2] = vertices();

            if (clockwise)
                std::swap(p1, p2);

            auto edge1 = p1 - p0;
            auto edge2 = p2 - p0;

            auto pvec = tao::cross(r_direction, edge2);
            auto det = tao::dot(edge1, pvec);

            Point2 uv;

            if (bfc) {
                if (det < EPSILON)
                    return false;

                auto tvec = r_origin - p0;

                auto u = tao::dot(tvec, pvec);
                if (u < 0.0 || u > det)
                    return false;
                uv(0) = u;

                auto qvec = tao::cross(tvec, edge1);
                auto v = tao::dot(r_direction, qvec);
                if (v < 0.0 || u + v > det)
                    return false;
                uv(1) = v;
                
                *tHit = tao::dot(edge2, qvec);
                auto inv_det = 1.0 / det;
                *tHit *= inv_det;
                uv(0) *= inv_det;
                uv(1) *= inv_det;

               if (*tHit < EPSILON)
                   return false;
            } else {
                
               if (det > -EPSILON && det < EPSILON) 
                   return false;
               auto inv_det = 1.0 / det;
               auto tvec = r_origin - p0;

               auto u = tao::dot(tvec, pvec) * inv_det;
               if (u < 0.0 || u > 1.0)
                   return false;
               uv(0) = u;

               auto qvec = tao::cross(tvec, edge1);

               auto v = tao::dot(r_direction, qvec) * inv_det;
               if (v < 0.0 || u + v > 1.0)
                   return false;
               uv(1) = v;

               *tHit = tao::dot(edge2, qvec) * inv_det;

               if (*tHit < EPSILON)
                  return false;
            }

            if (*tHit > ray.tMax)
                return false;

            if (hit_record != nullptr) {
                hit_record->_p = ray(*tHit);
                hit_record->_uv = uv;
                hit_record->_wo = -1.0f*ray.get_direction();
                hit_record->_t = *tHit;

                auto [n0, n1, n2] = normals();

                if (clockwise)
                    std::swap(n1, n2);

                hit_record->_n = tao::unitize((1-uv(0)-uv(1))*n0 + uv(0)*n1 + uv(1)*n2);
            }

            return true;
        }

        bool intersect(const Ray& ray) override {
            float t;
            return this->intersect(ray, &t, nullptr);
        }

        std::tuple<Point3, Point3, Point3> vertices() const {
            auto p0 = mesh->points[v[0]];
            auto p1 = mesh->points[v[1]];
            auto p2 = mesh->points[v[2]];
            return {p0, p1, p2};
        }

        std::tuple<Vec3, Vec3, Vec3> normals() const {
            auto n0 = mesh->normals[v[0]];
            auto n1 = mesh->normals[v[1]];
            auto n2 = mesh->normals[v[2]];
            return {n0, n1, n2};
        }

        Bounds3 world_bound() const override {
            auto [p0, p1, p2] = vertices();
            auto bounds = Bounds3{p0, p1};
            return bounds.get_union(bounds, p2);
        }
};
};

#endif

