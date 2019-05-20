#ifndef _OMG_DIRECTIONAL_LIGHT_
#define _OMG_DIRECTIONAL_LIGHT_

#include "omg/common.h"

namespace omg {

/**
 * Represents a directional source of light.
 *
 * @author Vitor Greati
 * */
class DirectionalLight : public Light {

    private:

        Vec3 _direction;

    public:

        DirectionalLight(const Vec3& intensity, const Vec3& direction)
            : Light {intensity}, _direction {direction} { /* empty */ }


        /**
         * Sample a point on the light source's surface.
         *
         * */
        virtual Vec3 sample_li(const SurfaceInteraction& interaction,
                Vec3 *wi, VisibilityTester* vt) const {
            *wi = tao::unitize(-_direction);
            return _intensity;
        }

        /**
         * Returns the direction of the light, when it possibly depends
         * on the intersection position over the surface.
         *
         * @param position the intersection position
         * @return light direction
         * */
        virtual std::optional<Vec3> get_direction(const Vec3& surface_point) const {
            return tao::unitize(_direction);
        }

};

};

#endif
