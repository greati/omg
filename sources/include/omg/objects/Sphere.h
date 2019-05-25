#ifndef _OMG_SPHERE_
#define _OMG_SPHERE_

#include "omg/objects/Object.h"
#include <cmath>

namespace omg {
/**
 * Represents a sphere.
 *
 * @author Vitor Greati
 * */
class Sphere : public Object {

    private:

        float _radius;        /** Sphere radius */
        Point3 _center;       /** Sphere center */

    public:

        /**
         * Basic constructor.
         * */
        Sphere(float radius, const Point3& center)
            : Object{}, _radius {radius}, _center {center}
        {/* empty */}

        /**
         * Check whether the sphere intersect
         * with a ray.
         *
         * */
        bool intersect(const Ray& ray, float * tHit, SurfaceInteraction* hit_record) override {
            const auto& origin = ray.get_origin();
            const auto& direction = ray.get_direction();        

            const auto& v = origin - _center;

            auto A = tao::dot(direction, direction);
            auto B = 2.0f * tao::dot(v, direction);
            auto C = tao::dot(v, v) - (_radius * _radius);

            auto delta = B * B - 4.0 * A * C;

            if (delta < 0.0) return false;

            float t1 = (-B + std::sqrt(delta)) / (2 * A);
            float t2 = (-B - std::sqrt(delta)) / (2 * A);

            float tShapeHit = std::min(t1, t2);

            if (tShapeHit > ray.tMax || tShapeHit < 0)
               return false; 

            *tHit = tShapeHit;

            if (hit_record != nullptr) {
                hit_record->_t = tShapeHit;
                hit_record->_p = ray(hit_record->_t);
                hit_record->_n = 2.0f * (hit_record->_p - _center);
                hit_record->_wo = -1.0f * (direction - origin);
            }

            return true;
        }

        bool intersect(const Ray& ray) override {
            const auto& origin = ray.get_origin();
            const auto& direction = ray.get_direction();        

            const auto& v = origin - _center;

            auto A = tao::dot(direction, direction);
            auto B = 2.0f * tao::dot(v, direction);
            auto C = tao::dot(v, v) - (_radius * _radius);

            auto delta = B * B - 4.0 * A * C;

            if (delta < 0.0) return false;

            float t1 = (-B + std::sqrt(delta)) / (2 * A);
            float t2 = (-B - std::sqrt(delta)) / (2 * A);

            float tShapeHit = std::min(t1, t2);

            if (tShapeHit > ray.tMax || tShapeHit < 0)
               return false; 

            return true;
        }

        Bounds3 world_bound() const {
            Vec3 radiuses {_radius, _radius, _radius};
            auto upper_corner = _center + radiuses;
            auto lower_corner = _center - radiuses;
            return Bounds3 {lower_corner, upper_corner};
        }
};
};

#endif

