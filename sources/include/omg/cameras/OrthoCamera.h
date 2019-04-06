#ifndef __ORTHOCAMERA__
#define __ORTHOCAMERA__

#include "Camera.h"

namespace omg {
/**
 * Represents an orthogonal camera.
 *
 * @author Vitor Greati
 * */
class OrthoCamera : public Camera {

    public:

        OrthoCamera(int width, int height, const VpDims& vpdims)
            : Camera {width, height, vpdims}
        {/*empty*/}


        Ray generate_ray(float x, float y) override {

        }
};
};
#endif
