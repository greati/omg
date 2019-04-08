#ifndef __PERSPECTIVECAMERA__
#define __PERSPECTIVECAMERA__

#include "Camera.h"
#include <cmath>

namespace omg {
/**
 * Represents a perspective camera.
 *
 * @author Vitor Greati
 * */
class PerspectiveCamera : public Camera {

    private:

        float _d;           /** Focal distance */
        float _vaov;        /** Vertical angle of view */

    public:

        PerspectiveCamera(int width, int height, const Point3& position, const Vec3& target, const Vec3& up, const VpDims& vpdims, float d=1.0)
            : Camera {width, height, position, target, up, vpdims}, _d {d}
        {/*empty*/}

        PerspectiveCamera(int width, int height, const Point3& position, const Vec3& target, const Vec3& up, float d=1.0, float vaov = 90.0)
            : Camera {width, height, position, target, up}, _d {d}, _vaov {vaov}
        {
            float hh = std::tan(vaov / 2);
            float hw = this->_aspect_ratio * hh;
            Vec3 llc = this->_position - d*_w - hw*_u;
            Vec3 vph = 2*hw*_u;
            Vec3 vpv = 2*hh*_v;

        }

        Ray generate_ray(float x, float y) override {
            Point3 origin = _position;
            auto [u, v] = this->compute_u_v(x, y);
            Vec3 direction = -_d * _w + (u * _u) + (v * _v);        
            return Ray {origin, direction};
        }
};
};
#endif
