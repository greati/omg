#ifndef _AGGPRIMITIVE_
#define _AGGPRIMITIVE_

#include "omg/raytracer/Ray.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/materials/Material.h"
#include "omg/objects/Primitive.h"

namespace omg {
/**
 * Represents an aggregate of primitives to be rendered
 * in the screen.
 *
 * @author Vitor Greati
 * */
class AggregatePrimitive : public Primitive {

    public:
        /**
         * Determines if the object intersects
         * with a ray.
         *
         * @param ray the ray
         * @param interaction collect information
         * @return if the object intersects with the ray
         * */
        bool intersect(const Ray& ray, SurfaceInteraction* interaction) const override {
            return false; 
        }

        /**
         * Only check if intersection occurred.
         *
         * @param ray the ray
         * @return if intersection occurred
         * */
        bool intersect(const Ray& ray) const override {
            return false; 
        }

};
};
#endif
