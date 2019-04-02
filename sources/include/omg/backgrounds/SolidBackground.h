#ifndef __SOLIDBACKGROUND__
#define __SOLIDBACKGROUND__

#include "omg/common.h"
#include "Background.h"

/**
 * Solid background.
 *
 * @author Vitor Greati
 * */

namespace omg {
class SolidBackground : public Background {

    private:

        RGBColor color;

    public:

        SolidBackground (const RGBColor & _color)
        : color {_color} {/* empty */}

        RGBColor find(float x, float y) const override {
            return color;
        }

};
};

#endif
