#ifndef __RAY__
#define __RAY__

#include "omg/common.h"

namespace omg {
/**
 * Represents a ray in the raytracer 
 * technique.
 *
 * @author Vitor Greati
 * */
class Ray {

    private:

        Point3 _o;       /** Ray origin */
        Vec3 _d;         /** Ray direction */

    public:

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
        inline Point3 operator()(float t) { return _o + _d * t; }

        /**
         * Ray origin
         *
         * @return origin
         * */
        inline Vec3 get_origin() { return _o; }

        /**
         * Ray direction
         *
         * @return direction
         * */
        inline Vec3 get_direction() { return _d; }

};
};
#endif
