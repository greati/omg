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
         * Sample a point on the light source's surface.
         *
         * */
        virtual Vec3 sample_li(const SurfaceInteraction& interaction,
                Vec3 *wi, VisibilityTester* vt) const {
            *wi = tao::unitize(_position - interaction._p);
            return _intensity;
        }

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
