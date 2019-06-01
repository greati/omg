#ifndef _BVH_ACCEL_PRIMITIVE_
#define _BVH_ACCEL_PRIMITIVE_

#include "omg/raytracer/Ray.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/materials/Material.h"
#include "omg/objects/Bounds3.h"
#include "omg/objects/AggregatePrimitive.h"

namespace omg {
/**
 * Bounded Volumes Hierarchy ray tracing
 * acceleration.
 *
 * @author Vitor Greati
 * */
class BVHAccel : public AggregatePrimitive {

    public:

        virtual bool intersect(const Ray& ray, SurfaceInteraction* interaction) const;

        virtual bool intersect(const Ray& ray) const;

        virtual const Material* get_material() const {
            return nullptr;
        }

        Bounds3 world_bound() const;

};
};
#endif
