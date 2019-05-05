#ifndef _GPRIMITIVE_
#define _GPRIMITIVE_

#include "omg/raytracer/Ray.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/materials/Material.h"
#include "omg/objects/Primitive.h" 

namespace omg {
/**
 * Represents a primitive to be rendered
 * in the screen.
 *
 * @author Vitor Greati
 * */
class GeometricPrimitive : public Primitive {

    private:

        std::shared_ptr<Object> _object;

        std::shared_ptr<Material> _material;

    public:

        GeometricPrimitive(const std::shared_ptr<Object>& object,
                const std::shared_ptr<Material>& material) : _object {object}, _material {material} {}

        inline Object* get_object() const { return _object.get(); }

        const Material* get_material() const override { return _material.get(); }

        /**
         * Determines if the object intersects
         * with a ray.
         *
         * @param ray the ray
         * @param interaction collect information
         * @return if the object intersects with the ray
         * */
        bool intersect(const Ray& ray, SurfaceInteraction* interaction) const override {
            float tHit;
            
            if (!_object->intersect(ray, &tHit, interaction))
                return false;

            ray.tMax = tHit;
            interaction->_primitive = this;
            return true;
        }

        /**
         * Only check if intersection occurred.
         *
         * @param ray the ray
         * @return if intersection occurred
         * */
        bool intersect(const Ray& ray) const override {
            return _object->intersect(ray); 
        }

};
};
#endif
