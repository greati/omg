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
        
    private:

        std::shared_ptr<Material> _material;

    public:

        /**
         * Return a pointer to the material.
         *
         * @return pointer to object's material
         * */
        inline const std::shared_ptr<Material> get_material(void) const { return _material; }

        /**
         * Determines if the object intersects
         * with a ray.
         *
         * @param ray the ray
         * @param interaction collect information
         * @return if the object intersects with the ray
         * */
        virtual bool intersect(const Ray& ray, SurfaceInteraction* interaction) = 0;

        /**
         * Only check if intersection occurred.
         *
         * @param ray the ray
         * @return if intersection occurred
         * */
        virtual bool intersect(const Ray& ray) = 0;

};
};
#endif

