#ifndef __CAMERA__
#define __CAMERA__

#include "omg/raytracer/Ray.h"
#include "linalg/VecOperations.h"

namespace omg {
/**
 * Represents a generic camera.
 *
 * @author Vitor Greati
 * */
class Camera {

    public:

        /**
         * View plane dimensions.
         *  
         *             t
         *   ---------------------
         *  |                     |
         *l |          0          |r  
         *  |                     |
         *   ---------------------
         *             b
         * */
        struct VpDims {
            float l, r, b, t;

            VpDims() {/*empty*/}

            VpDims (float l, float r, float b, float t)
                : l{l}, r{r}, b{b}, t{t} {/*empty*/}
        };

    protected:

        Point3 _position;    /** Camera position */
        Vec3 _target;        /** Camera direction */
        Vec3 _up;            /** Camera up direction */
        VpDims _vpdims;      /** Virtual plane dimensions */
        int _width;          /** Camera width */
        int _height;         /** Camera height */
        Vec3 _u, _v, _w;     /** Orthogonal basis */

    public:

        Camera(int width, int height)
            : _width {width}, _height {height}
        {/*empty*/}

        Camera(int width, int height, const VpDims& vpdims)
            : _width {width}, _height {height}, _vpdims {vpdims}
        {/*empty*/}

        Camera(int width, int height, const VpDims& vpdims, 
                const Point3& position, const Vec3& target, const Vec3& up)
            : _width {width}, _height {height}, _vpdims{vpdims}, _position{position}, _target{target}, _up{up}
        {/*empty*/}

        Camera(int width, int height, const Point3& position, const Vec3& target, const Vec3& up)
            : _width {width}, _height {height}, _position{position}, _target{target}, _up{up}
        {
            _v = _up;
            _u = tao::cross(_v, _target - _position);
            _w = tao::cross(_u, _v);
        }

        /**
         * Produces a ray given an x and y in [0,1].
         *
         * @param x column walking
         * @param y row walking
         * @return a ray
         * */
        virtual Ray generate_ray(float x, float y) = 0;

        /**
         * The scene width
         *
         * @return the scene width
         * */
        int get_width() const { return this->_width; }

        /**
         * The scene height
         *
         * @return the scene height
         * */
        int get_height() const { return this->_height; }

        /**
         * Set the scene width
         *
         * @param width the scene width
         * */
        void set_width(int width) { this->_width = width; }

        /**
         * Set the scene height
         *
         * @param width the scene height
         * */
        void set_height(int height) { this->_height = height; }
};
};
#endif
