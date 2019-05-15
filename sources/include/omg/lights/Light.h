#ifndef _OMG_LIGHT_
#define _OMG_LIGHT_

#include "omg/common.h"

namespace omg {
/**
 * Represents a light.
 *
 * @author Vitor Greati
 * */
class Light {

    private:

        Vec3 _intensity;

    public:

        Light(const Vec3& intensity)
            : _intensity {intensity} { /* empty */ }

        /**
         * Returns the direction of the light, when it doesn't
         * depend on any other parameter.
         *
         * @return light direction if suitable
         * */
        virtual std::optional<Vec3> get_direction() const {
            return std::nullopt;
        }

        /**
         * Returns the direction of the light, when it depends
         * on the intersection position over the surface.
         *
         * @param position the intersection position
         * @return light direction if suitable
         * */
        virtual std::optional<Vec3> get_direction(const Vec3& surface_point) const {
            return std::nullopt;
        }
};

};
#endif
