#ifndef _OMG_COMMON_
#define _OMG_COMMON_

#include <array>
#include <tuple>
#include "omg/globals.h"
#include "tao/core.h"
#include "logger/StandardLogger.h"

namespace omg {

    using RealValue = float;

    using Matrix4x4 = tao::Mat<float, 4, 4>;

    /** Represents a 3d vector */
    using Vec3 = tao::Vec3<RealValue>;

    /** Represents a 3d vector */
    using Vec2 = tao::Vec2<RealValue>;

    /** Represents an RGB color */
    using RGBColor = Vec3;

    /** Represents a point in 3 dimentions */
    using Point3 = Vec3;

    /** Represents a point in 2 dimentions */
    using Point2 = tao::Vec2<RealValue>;

    using Point2i = std::pair<int, int>;

    template<int NChannels>
    using PixelValue = std::array<unsigned char, NChannels>;

    template<int N>
    using Size = std::array<float, N>;

    static StandardLogger logger {"[omg]"};

};

#endif
