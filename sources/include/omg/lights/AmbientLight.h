#ifndef _OMG_AMBIENT_LIGHT_
#define _OMG_AMBIENT_LIGHT_

#include "omg/common.h"

namespace omg {

/**
 * Represents a point light.
 *
 * @author Vitor Greati
 * */
class AmbientLight : public Light {

    public:

        AmbientLight(const Vec3& intensity)
            : Light {intensity} { /* empty */ }

        virtual bool is_ambient() const { return true; }


        /**
         * Sample a point on the light source's surface.
         *
         * */
        virtual Vec3 sample_li(const SurfaceInteraction& interaction,
                Vec3 *wi) const {
            return _intensity;
        }
};

};

#endif
