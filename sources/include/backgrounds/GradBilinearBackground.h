#ifndef __GRADBACKGROUND__
#define __GRADBACKGROUND__

#include "common.h"
#include "Background.h"

namespace rayt {
/**
 * Gradient background based on bilinear interpolation.
 *
 * @author Vitor Greati
 * */
class GradBilinearBackground : public Background {

    private:

        RGBColor ulc, urc, llc, lrc;

    public:

        GradBilinearBackground(const RGBColor & _ulc,
                       const RGBColor & _urc,
                       const RGBColor & _lrc,
                       const RGBColor & _llc)
        : ulc {_ulc}, urc {_urc}, llc {_llc}, lrc {_lrc} {/* empty */}

        RGBColor find(float x, float y) const override {

            float x1 = 0.0;
            float y1 = 0.0;
            float x2 = 1.0;
            float y2 = 1.0;

            auto [r11, g11, b11] = std::tuple{llc(0), llc(1), llc(2)};
            auto [r12, g12, b12] = std::tuple{ulc(0), ulc(1), ulc(2)};
            auto [r21, g21, b21] = std::tuple{lrc(0), lrc(1), lrc(2)};
            auto [r22, g22, b22] = std::tuple(urc(0), urc(1), urc(2));

            return {
                bilinear_interp(x, y, x1, x2, y1, y2, r11, r12, r21, r22),
                bilinear_interp(x, y, x1, x2, y1, y2, g11, g12, g21, g22),
                bilinear_interp(x, y, x1, x2, y1, y2, b11, b12, b21, b22),
            };
        };

        float bilinear_interp(float x, float y, float x1, float x2, float y1, float y2,
                float v11, float v12, float v21, float v22) const {
            float f1 = 1.0 / ((x2-x1)*(y2-y1));
            float f2 = v11 * (x2 - x) * (y2 - y);
            float f3 = v21 * (x - x1) * (y2 - y);
            float f4 = v12 * (x2 - x) * (y - y1);
            float f5 = v22 * (x - x1) * (y - y1);
            return f1 * (f2 + f3 + f4 + f5);
        }

};
};

#endif