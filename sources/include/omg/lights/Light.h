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

    protected:

        Vec3 _intensity;

    public:

        Light(const Vec3& intensity)
            : _intensity {intensity} { /* empty */ }

        /**
         * Sample a point on the light source's surface.
         *
         * */
        virtual Vec3 sample_li(const SurfaceInteraction& interaction,
                Vec3 *wi) const = 0;

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

        virtual bool is_ambient() const { return false; }

        const Vec3& get_intensity() const { return _intensity; }
};

};
#endif
