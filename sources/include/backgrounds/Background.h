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
         * @param p the point
         * */
        virtual Color find(const Point2& p) const = 0;

};
};

#endif

