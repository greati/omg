#ifndef __PERSPECTIVECAMERA__
#define __PERSPECTIVECAMERA__

#include "Camera.h"

namespace omg {
/**
 * Represents a perspective camera.
 *
 * @author Vitor Greati
 * */
class PerspectiveCamera : public Camera {

    public:

        PerspectiveCamera(int width, int height)
            : Camera {width, height}
        {/*empty*/}

        Ray generate_ray(float x, float y) override {
        
        }
};
};
#endif
