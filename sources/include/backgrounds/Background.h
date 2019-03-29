#ifndef __BACKGROUND__
#define __BACKGROUND__

#include "common.h"

/**
 * Interface for backgrounds.
 *
 * @author Vitor Greati
 * */
namespace rayt {
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

