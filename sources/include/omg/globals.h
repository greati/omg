#ifndef _OMG_GLOBALS_
#define _OMG_GLOBALS_

#include <limits>

namespace omg {

    inline static constexpr float MachineEpsilon =
        std::numeric_limits<float>::epsilon() * 0.5;

    inline constexpr float gamma(int n) {
        return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
    }


};

#endif
