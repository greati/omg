#ifndef __RAY__
#define __RAY__

#include "omg/common.h"
#include <limits>


namespace omg {
/**
 * Represents a ray in the raytracer 
 * technique.
 *
 * @author Vitor Greati
 * */
class Ray {

    private:

        Point3 _o;                                          /** Ray origin */
        Vec3 _d;                                            /** Ray direction */

    public:

        mutable float tMax {std::numeric_limits<float>::max()};     /** Max distance that the ray achieved */

        /**
         * Basic constructor.
         *
         * */
        Ray (const Point3& o, const Vec3& d) : _o {o}, _d {d} {/*empty*/}

        /**
         * Gives the point p of the ray for parameter t,
         * where p = o + d*t
         *
         * @param t real parameter
         * @return the point at o+d*t
         * */
        inline Point3 operator()(float t) const { return _o + _d * t; }

        /**
         * Ray origin
         *
         * @return origin
         * */
        inline Vec3 get_origin() const { return _o; }

        /**
         * Ray direction
         *
         * @return direction
         * */
        inline Vec3 get_direction() const { return _d; }

        inline std::pair<float, float> get_spherical_angles() const {
            auto phi = tao::spherical_phi(_d);
            auto theta = tao::spherical_theta(_d);
            return {phi, theta};
        }
};
};
#endif
