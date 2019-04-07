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

    private:

        float _d;           /** Focal distance */

    public:

        PerspectiveCamera(int width, int height, const VpDims& vpdims, const Point3& position, const Vec3& target, const Vec3& up, float d=1.0)
            : Camera {width, height, vpdims, position, target, up}, _d {d}
        {/*empty*/}

        PerspectiveCamera(int width, int height, float d = 1.0)
           : Camera {width, height}, _d {d}
        {/*empty*/}

        Ray generate_ray(float x, float y) override {
            Point3 origin = _position;
            auto [u, v] = this->compute_u_v(x, y);
            Vec3 direction = -_d * _w + (u * _u) + (v * _v);        
            return Ray {origin, direction};
        }
};
};
#endif
