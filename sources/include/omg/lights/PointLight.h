#ifndef _OMG_POINT_LIGHT_
#define _OMG_POINT_LIGHT_

#include "omg/common.h"

namespace omg {

/**
 * Represents a point light.
 *
 * @author Vitor Greati
 * */
class PointLight : public Light {

    private:

        Vec3 _position;

    public:

        PointLight(const Vec3& intensity, const Vec3& position)
            : Light {intensity}, _position {position} { /* empty */ }

        /**
         * Returns the direction of the light, when it depends
         * on the intersection position over the surface.
         *
         * @param position the intersection position
         * @return light direction
         * */
        virtual std::optional<Vec3> get_direction(const Vec3& surface_point) const {
            return tao::unitize(surface_point - _position);
        }

};

};

#endif
