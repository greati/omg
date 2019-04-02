#ifndef __OMG_COMMON__
#define __OMG_COMMON__

#include <array>
#include <tuple>
#include "linalg.h"
#include "logger/StandardLogger.h"

namespace omg {

    using RGBColor = tao::VecF;

    template<int NChannels>
    using PixelValue = std::array<unsigned char, NChannels>;

    template<int N>
    using Size = std::array<float, N>;

    static StandardLogger logger {"[omg]"};

};

#endif
