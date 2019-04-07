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
        Vec3 _u, _v, _w;     /** Orthonormal basis */

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
        {
            _w = -1.0f * (_target - _position).unit().value();
            _u = tao::cross(_up, _w).unit().value();
            _v = tao::cross(_w, _u).unit().value();
        }

        Camera(int width, int height, const Point3& position, const Vec3& target, const Vec3& up)
            : _width {width}, _height {height}, _position{position}, _target{target}, _up{up}
        {
            _w = -1.0f * (_target - _position).unit().value();
            _u = tao::cross(_up, _w).unit().value();
            _v = tao::cross(_w, _u).unit().value();
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

        
        /** 
         * Compute the (u,v) from (i,j), i, j in [0,1]
         *
         * @param i horizontal displacement
         * @param j vertical displacement
         * @return the pair (u, v)
         * */
        std::pair<float, float> compute_u_v(float i, float j) const {
            float ii = i * _width;
            float jj = j * _height;
            auto vd = this->_vpdims;
            auto u = vd.l + (vd.r - vd.l) * (ii + 0.5) / _width;
            auto v = vd.b + (vd.t - vd.b) * (jj + 0.5) / _height;
            return {u, v};
        }

        /**
         * Get the associated orthonormal basis.
         *
         * @return the orthonormal basis
         * */
        inline std::array<Vec3, 3> get_ortho_basis() {
            return {_u, _v, _w};
        }

};
};
#endif
