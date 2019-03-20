#ifndef __GRADBACKGROUND__
#define __GRADBACKGROUND__

#include "common.h"
#include "Background.h"

/**
 * Gradient background.
 *
 * @author Vitor Greati
 * */

namespace rayt {
class GradBackground : public Background {

    private:

        Color ulc, urc, llc, lrc;
        Size<2> size;

    public:

        GradBackground (const Size<2>& _size,
                        const Color & _ulc,
                        const Color & _urc,
                        const Color & _llc,
                        const Color & _lrc)
        : ulc {_ulc}, urc {_urc}, llc {_llc}, lrc {_llc}, size {_size} {/* empty */}

        Color find(const Point2& p) const override {
            auto [x, y] = p;
            auto [w, h] = size;

            float x1 = 0;
            float y1 = 0;
            float x2 = w-1;
            float y2 = h-1;

            auto [r11, g11, b11] = llc;
            auto [r21, g21, b21] = ulc;
            auto [r12, g12, b12] = lrc;
            auto [r22, g22, b22] = urc;

            return {
                bilinear_interp(x, y, x1, x2, y1, y2, r11, r12, r21, r22),
                bilinear_interp(x, y, x1, x2, y1, y2, g11, g12, g21, g22),
                bilinear_interp(x, y, x1, x2, y1, y2, b11, b12, b21, b22),
            };
        };

        float bilinear_interp(float x, float y, float x1, float x2, float y1, float y2,
                float v11, float v12, float v21, float v22) const {
            float f1 = 1.0 / (x2-x1)*(y2-y1);
            float f2 = v11 * (x2 - x) * (y2 - y);
            float f3 = v21 * (x - x1) * (y2 - y);
            float f4 = v12 * (x2 - x) * (y - y1);
            float f5 = v22 * (x - x1) * (y - y1);
            return f1 * (f2 + f3 + f4 + f5);
        }

};
};

#endif
