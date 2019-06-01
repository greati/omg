#ifndef _AGGLISTPRIMITIVE_
#define _AGGLISTPRIMITIVE_

#include "omg/raytracer/Ray.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/materials/Material.h"
#include "omg/objects/AggregatePrimitive.h"

namespace omg {
/**
 * Represents an aggregate of primitives to be rendered
 * in the screen.
 *
 * @author Vitor Greati
 * */
class ListAggregate : public AggregatePrimitive {

    private:

        std::vector<std::shared_ptr<Primitive>> _primitives;  /** Scene objects */

    public:

        ListAggregate(const decltype(_primitives)& primitives)
            : _primitives {primitives} {/*empty*/}

        /**
         * Determines if the object intersects
         * with a ray.
         *
         * @param ray the ray
         * @param interaction collect information
         * @return if the object intersects with the ray
         * */
        bool intersect(const Ray& ray, SurfaceInteraction* si) const {
            for (auto & o : _primitives) 
                o->intersect(ray, si);
            return (si->_primitive != nullptr);
        }

        /**
         * Only check if intersection occurred.
         *
         * @param ray the ray
         * @return if intersection occurred
         * */
        virtual bool intersect(const Ray& ray) const {
            for (auto & o : _primitives) 
                if (o->intersect(ray))
                    return true;
            return false;
        }

        Bounds3 world_bound() const override {
            Bounds3 wb;
            for (auto& p : _primitives)
                wb = wb.get_union(wb, p->world_bound());
            return wb;
        }

};
};
#endif
