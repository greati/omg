#ifndef _OMG_OBJ_
#define _OMG_OBJ_

#include "omg/raytracer/Ray.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/materials/Material.h"
#include "omg/objects/Transform.h"
#include "Bounds3.h"

namespace omg {
/**
 * Represents an object.
 *
 * @author Vitor Greati
 * */
class Object {

    public:

        const std::shared_ptr<Transform> object_to_world, world_to_object;

        Object() {} //TODO remove when transforms are loaded properly

        Object(const std::shared_ptr<Transform>& object_to_world,
                const std::shared_ptr<Transform>& world_to_object)
        : object_to_world {object_to_world}, world_to_object {world_to_object} {}

        virtual bool intersect(const Ray& ray, float * tHit, SurfaceInteraction* hit_record) = 0;

        virtual bool intersect(const Ray& ray) = 0;

        virtual Bounds3 world_bound() const = 0;
};
};
#endif

