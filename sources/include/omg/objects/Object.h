#ifndef _OMG_OBJ_
#define _OMG_OBJ_

#include "omg/raytracer/Ray.h"

namespace omg {
/**
 * Represents an object.
 *
 * @author Vitor Greati
 * */
class Object {
    
    public:

        /**
         * Data regarding object hits by some ray.
         * */
        struct HitRecord {
            float t;            /** The ray parameter where the hit occurred */

            HitRecord() : t {-1.0} {/*empty*/}

            HitRecord(float _t)
                : t {_t}
            {/*empty*/}
        };

        /**
         * Determines if the object intersects
         * with a ray.
         *
         * @param ray the ray
         * @return if the object intersects with the ray
         * */
        virtual bool intersect(Ray ray, Object::HitRecord& hit_record) = 0;

};
};
#endif

