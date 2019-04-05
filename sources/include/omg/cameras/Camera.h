#ifndef __CAMERA__
#define __CAMERA__

#include "omg/raytracer/Ray.h"

namespace omg {
/**
 * Represents a camera.
 *
 * @author Vitor Greati
 * */
class Camera {

    private:

        int _width;          /** Camera width */
        int _height;         /** Camera height */

    public:


        Camera(int width, int height)
            : _width {width}, _height {height}
        {/*empty*/}

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
