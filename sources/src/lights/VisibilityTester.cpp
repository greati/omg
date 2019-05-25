#include "omg/lights/VisibilityTester.h"

using namespace omg;

bool VisibilityTester::unoccluded(const Scene& scene) const {
    return !scene.intersect(si0.spaw_ray_to(si1));
}
