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


        PerspectiveCamera(int width, int height, const Point3& position, const Vec3& target, 
                const Vec3& up, float aspect_ratio, float d, float vaov)
            : Camera {width, height, position, target, up, aspect_ratio}, _d {d}, _vaov {vaov}
        {
            init(); 
        }

        PerspectiveCamera(int width, int height, const Point3& position, const Vec3& target, 
                const Vec3& up, float d, float vaov)
            : Camera {width, height, position, target, up}, _d {d}, _vaov {vaov}
        {
            init();
        }

        void init() {
            float hh = std::tan(M_PI/180.0 * (this->_vaov / 2.0));
            float hw = this->_aspect_ratio * hh;
            this->_vpdims.l = -hw;
            this->_vpdims.r = hw;
            this->_vpdims.t = hh;
            this->_vpdims.b = -hh;
        }

        Ray generate_ray(float x, float y) override {
            Point3 origin = _position;
            auto [u, v] = this->compute_u_v(x, y);
            Vec3 direction = -_d * _w + (u * _u) + (v * _v);        
            return Ray {origin, direction.unit().value()};
        }
};
};
#endif
