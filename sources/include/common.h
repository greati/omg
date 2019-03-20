#ifndef __COMMON__
#define __COMMON__

#include <array>
#include <tuple>

namespace rayt {

    using Vec2 = std::tuple<float, float>;
    using Point2 = std::tuple<float, float>;
    using Vec3 = std::tuple<float, float, float>;
    using Point3 = Vec3; 
    using Color = std::tuple<float, float, float>;

    template<int N>
    using Size = std::array<float, N>;

};

#endif
