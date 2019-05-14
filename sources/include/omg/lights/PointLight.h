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

};

};

#endif
