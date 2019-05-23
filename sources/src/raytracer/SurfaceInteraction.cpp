#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/raytracer/Ray.h"

using namespace omg;

Ray SurfaceInteraction::spaw_ray_to(const SurfaceInteraction& si1) const {
   auto normal = tao::unitize(_n);
   Point3 origin = _p + 0.001f*normal;
   Point3 to = si1._p;
   Vec3 d = to - origin;
   return Ray {origin, d};
}




