#ifndef _OMG_OBJ_
#define _OMG_OBJ_

#include "omg/raytracer/Ray.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/materials/Material.h"

namespace omg {
/**
 * Represents an object.
 *
 * @author Vitor Greati
 * */
class Object {

    public:

        Object() {}

        virtual bool intersect(const Ray& ray, SurfaceInteraction* hit_record) = 0;

        virtual bool intersect(const Ray& ray) = 0;
};
};
#endif

