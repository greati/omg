#ifndef __CAMERA__
#define __CAMERA__

#include "omg/raytracer/Ray.h"
#include "tao/core.h"
#include "omg/cameras/Film.h"

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
         *  
         *  l---------------------r
         *  |                     |t
         *  |          o          |  
         *  |                     |
         *  ----------------------b
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
        float _aspect_ratio;  /** Aspect ratio */
        int _width;          /** Camera width */
        int _height;         /** Camera height */
        Vec3 _u, _v, _w;     /** Orthonormal basis */
        Vec3 _plane_normal;  /** Image plane normal */
        std::unique_ptr<Film> _film;    /** Film to display the image */

    public:

        Camera(int width, int height,
               const Point3& position, const Vec3& target, const Vec3& up, 
               const VpDims& vpdims,
               const float aspect_ratio)
            : Camera {width, height, position, target, up, vpdims}
        {
            this->_aspect_ratio = aspect_ratio;
        }

        Camera(int width, int height,
               const Point3& position, const Vec3& target, const Vec3& up, 
               const VpDims& vpdims)
            : Camera {width, height, position, target, up}
        {
            this->_vpdims = vpdims;
        }

        Camera(int width, int height, const Point3& position, const Vec3& target, const Vec3& up)
            : _width {width}, _height {height}, _position {position}, _target {target}, _up {up}
        {
            this->_aspect_ratio = static_cast<float>(_width) / static_cast<float>(_height);

            _w = -1.0f * tao::unitize(_target - _position);
            _u = tao::unitize(tao::cross(_up, _w));
            _v = tao::unitize(tao::cross(_w, _u));

            this->_plane_normal = -1.0f * _w;
        }

        Camera(int width, int height, const Point3& position, const Vec3& target, const Vec3& up, float aspect_ratio)
            : Camera {width, height, position, target, up}
        {
            this->_aspect_ratio = aspect_ratio;
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
         * The aspect ratio.
         *
         * @return the aspect ratio
         * */
        inline float get_aspect_ratio() const { return this->_aspect_ratio; }
        
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
         * Compute the (i,j) from (u,v), u, v in [w, h]
         *
         * @param u horizontal displacement
         * @param v vertical displacement
         * @return the pair (i, j)
         * */
        std::pair<float, float> compute_i_j(float u, float v) const {
            auto vd = this->_vpdims;
            auto ii = ((u - vd.l) * _width) / (vd.r - vd.l) - 0.5;
            auto jj = ((v - vd.b) * _height) / (vd.t - vd.b) - 0.5;
            return {ii / _width, jj / _height};
        }

        /**
         * Get the associated orthonormal basis.
         *
         * @return the orthonormal basis
         * */
        inline std::array<Vec3, 3> get_ortho_basis() {
            return {_u, _v, _w};
        }

        /**
         * Set the image plane normal.
         *
         * @param normal the image plane normal
         * */
        inline void set_plane_normal(const Vec3& normal) { this->_plane_normal = normal; }

        /**
         * Set the film.
         *
         * @param film a pointer to a film
         * */
        inline void set_film(std::unique_ptr<Film> film) 
        { this->_film = std::move(film); }

        /**
         * Get the film.
         *
         * @return the film
         * */
        inline Film* get_film() { return _film.get(); }

};
};
#endif
