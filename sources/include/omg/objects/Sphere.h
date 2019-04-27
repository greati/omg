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
            : _radius {radius}, _center {center}
        {/* empty */}

        /**
         * Check whether the sphere intersect
         * with a ray.
         *
         * */
        bool intersect(const Ray& ray, SurfaceInteraction* hit_record) override {
            auto origin = ray.get_origin();
            auto direction = ray.get_direction();        

            auto A = tao::dot(direction, direction);
            auto B = 2.0f * tao::dot(origin - _center, direction);
            auto C = tao::dot(origin - _center, origin - _center) - (_radius * _radius);

            auto delta = B * B - 4.0 * A * C;

            if (delta < 0.0) return false;

            float t1 = (-B + std::sqrt(delta)) / (2 * A);
            float t2 = (-B - std::sqrt(delta)) / (2 * A);

            if (hit_record != nullptr) {
                hit_record->_t = std::min(t1, t2);
                //hit_record->_p = ray(hit_record->_t);
                //hit_record->_n = 2.0f * (hit_record->_p - _center);
                //hit_record->_wo = -1.0f * (direction - origin);
            }
            //hit_record->_object = std::shared_ptr<Sphere>(this);

            return true;
        }

        bool intersect(const Ray& ray) override {
            auto origin = ray.get_origin();
            auto direction = ray.get_direction();        

            auto A = tao::dot(direction, direction);
            auto B = 2.0f * tao::dot(origin - _center, direction);
            auto C = tao::dot(origin - _center, origin - _center) - (_radius * _radius);

            auto delta = B * B - 4.0 * A * C;

            return (delta >= 0.0);
        }
};
};

#endif

