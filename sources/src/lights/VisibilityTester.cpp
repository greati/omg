#include "omg/lights/VisibilityTester.h"

using namespace omg;

bool VisibilityTester::unoccluded(const Scene& scene) const {
	SurfaceInteraction si;
    return !scene.intersect(si0.spaw_ray_to(si1), &si);
}
