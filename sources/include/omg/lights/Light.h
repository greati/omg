#ifndef _OMG_LIGHT_
#define _OMG_LIGHT_

#include "omg/common.h"

namespace omg {

class VisibilityTester;

/**
 * Interface for lights.
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
         * @param interaction surface interaction
         * @param wi the vector towards the light
         * */
        virtual Vec3 sample_li(const SurfaceInteraction& interaction,
                Vec3 *wi, VisibilityTester* vt) const = 0;

        /**
         * Inform if this is an ambient light.
         *
         * @return is ambient light
         * */
        virtual bool is_ambient() const { return false; }

        /**
         * The light intensity (color).
         *
         * @return the light intensity
         * */
        const Vec3& get_intensity() const { return _intensity; }
};

};
#endif
