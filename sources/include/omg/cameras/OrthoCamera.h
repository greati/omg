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

        OrthoCamera(int width, int height, const Point3& position, const Vec3& target, const Vec3& up, const VpDims& vpdims)
            : Camera {width, height, position, target, up, vpdims}
        {/*empty*/}


        Ray generate_ray(float x, float y) override {
            Vec3 direction = -1.0f * this->_w;
            auto [u, v] = this->compute_u_v(x, y);
            Point3 origin = _position + (u * _u) + (v * _v);
            return Ray {origin, direction};
        }
};
};
#endif
