#include <iostream>
#include "backgrounds/GradBackground.h"

int main(void) {

    rayt::GradBackground grad {
        {300, 300},
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 255}
    };

    auto [x1, y1, z1] = grad.find({0.0, 0.0});
    auto [x2, y2, z2] = grad.find({0.0, 299.0});
    auto [x3, y3, z3] = grad.find({299.0, 0.0});
    auto [x4, y4, z4] = grad.find({299.0, 299.0});

    std::cout << (int) x1 << (int) y1 << (int) z1 << std::endl;
    std::cout << (int) x2 << (int) y2 << (int) z2 << std::endl;

    return 0;
}
