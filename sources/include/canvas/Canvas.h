#ifndef __CANVAS__
#define __CANVAS__

#include "common.h"
#include <new>

using namespace rayt;

/**
 * An interface for a canvas.
 *
 * @author Vitor Greati
 * @param CoordType type of coordinates
 * @param NChannels number of channels
 * @param NDim number of dimensions
 * */
template<typename CoordType, int NChannels = 3, int NDim=2>
class Canvas {

    public:

        /**
         * Set a value at a canvas position, throwing an exception
         * if the position is invalid.
         *
         * @param value the new value
         * */
        virtual void set(CoordType point, PixelValue<NChannels> value) const = 0;

        /**
         * Set a value at a canvas position, do not care about 
         * wrong position.
         *
         * @param value the new value
         * */
        virtual void set(CoordType point, PixelValue<NChannels> value, std::nothrow_t) const noexcept = 0;

        /**
         * Return the canvas height.
         *
         * @return the height
         * */
        virtual int height() const = 0;

        /**
         * Return the canvas width.
         *
         * @return the width
         * */
        virtual int width() const = 0;

        /**
         * Reset the canvas.
         *
         * @param size the new size
         * */
        virtual void reset(Size<NDim> size) = 0;

        /**
         * Clear the canvas with a value.
         *
         * @param value value to be set in the entire canvas
         * */
        virtual void clear(const PixelValue<NChannels> & value) = 0;

        /**
         * Access value at a position.
         *
         * @param point the position
         * @return the value
         * */
        virtual PixelValue<NChannels> at(CoordType point) const = 0;
};

#endif
