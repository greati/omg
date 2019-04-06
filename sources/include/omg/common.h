#ifndef __OMG_COMMON__
#define __OMG_COMMON__

#include <array>
#include <tuple>
#include "linalg.h"
#include "logger/StandardLogger.h"

namespace omg {

    /** Represents a 3d vector */
    using Vec3 = tao::VecF;

    /** Represents an RGB color */
    using RGBColor = tao::VecF;

    /** Represents a point in 3 dimentions */
    using Point3 = tao::VecF;

    template<int NChannels>
    using PixelValue = std::array<unsigned char, NChannels>;

    template<int N>
    using Size = std::array<float, N>;

    static StandardLogger logger {"[omg]"};

};

#endif
