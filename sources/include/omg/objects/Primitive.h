#ifndef _PRIMITIVE_
#define _PRIMITIVE_

#include "omg/raytracer/Ray.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/materials/Material.h"

/**
 * Represents a primitive to be rendered
 * in the screen.
 *
 * */
namespace omg {
class Primitive {

    public:

        /**
         * Return the material of the primitive.
         *
         * @return the material of the primitive
         * */
        virtual const Material* get_material() const = 0;

        /**
         * Determines if the object intersects
         * with a ray.
         *
         * @param ray the ray
         * @param interaction collect information
         * @return if the object intersects with the ray
         * */
        virtual bool intersect(const Ray& ray, SurfaceInteraction* interaction) const = 0;

        /**
         * Only check if intersection occurred.
         *
         * @param ray the ray
         * @return if intersection occurred
         * */
        virtual bool intersect(const Ray& ray) const = 0;

};
};
#endif
