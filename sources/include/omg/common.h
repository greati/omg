#ifndef __COMMON__
#define __COMMON__

#include <array>
#include <tuple>
#include "linalg.h"

namespace omg {

    using Vec2 = std::tuple<float, float>;
    using Point2 = std::tuple<float, float>;
    using Vec3 = std::tuple<float, float, float>;
    using Point3 = Vec3; 
    //using RGBColor = std::tuple<unsigned char, unsigned char, unsigned char>;
    using RGBColor = tao::VecF;


    template<int NChannels>
    using PixelValue = std::array<unsigned char, NChannels>;

    template<int N>
    using Size = std::array<float, N>;

};

#endif