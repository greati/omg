#ifndef __BACKGROUND__
#define __BACKGROUND__

#include "omg/common.h"

/**
 * Interface for backgrounds.
 *
 * @author Vitor Greati
 * */
namespace omg {
class Background {

    public:

        /**
         * Find a color in a given point.
         *
         * @param col the width percentage
         * @param row the height percentage
         * */
        virtual RGBColor find(float col, float row) const = 0;

};
};

#endif

