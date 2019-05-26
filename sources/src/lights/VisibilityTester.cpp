#include "omg/lights/VisibilityTester.h"

using namespace omg;

bool VisibilityTester::unoccluded(const Scene& scene) const {
    auto ray = si0.spaw_ray_to(si1);
    ray.tMax = 1.0f;
    return !scene.intersect(ray);
}
