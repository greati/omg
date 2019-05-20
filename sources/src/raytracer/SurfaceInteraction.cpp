#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/raytracer/Ray.h"

using namespace omg;

Ray SurfaceInteraction::spaw_ray_to(const SurfaceInteraction& si1) const {
   Point3 origin = _p;  //TODO apply offset
   Point3 to = si1._p;
   Vec3 d = to - origin;
   return Ray {origin, d};
}




