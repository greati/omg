#ifndef __SOLIDBACKGROUND__
#define __SOLIDBACKGROUND__

#include "common.h"
#include "Background.h"

/**
 * Solid background.
 *
 * @author Vitor Greati
 * */

namespace rayt {
class SolidBackground : public Background {

    private:

        RGBColor color;

    public:

        SolidBackground (const RGBColor & _color)
        : color {_color} {/* empty */}

        RGBColor find(const Point2& p) const override {
            return color;
        }

};
};

#endif
