#include "omg/accelerators/BVHAccel.h"

using namespace omg;


bool BVHAccel::intersect(const Ray& ray, SurfaceInteraction* interaction) const {
    return false;
}

bool BVHAccel::intersect(const Ray& ray) const {
    return false;
}

Bounds3 BVHAccel::world_bound() const {
    return Bounds3();
}


