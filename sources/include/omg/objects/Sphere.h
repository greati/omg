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
        bool intersect(Ray ray, Object::HitRecord& hit_record) override {
            auto origin = ray.get_origin();
            auto direction = ray.get_direction();        

            auto A = direction.dot(direction);
            auto B = 2.0f * (origin - _center).dot(direction);
            auto C = (origin - _center).dot(origin - _center) - (_radius * _radius);

            auto delta = B * B - 4.0 * A * C;

            if (delta < 0.0) return false;

            float t1 = (-B + std::sqrt(delta)) / (2 * A);
            float t2 = (-B - std::sqrt(delta)) / (2 * A);

            hit_record.t = std::min(t1, t2);

            return true;
        }
};
};

#endif
