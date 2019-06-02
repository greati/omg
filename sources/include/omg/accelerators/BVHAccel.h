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

        /**
         * Types of partition strategy.
         * */
        enum class SplitMethod {
            SAH,
            HLBVH,
            Middle,
            EqualCounts
        };

    private:

        const int _max_prims_node;
        const SplitMethod _split_method;
        std::vector<std::shared_ptr<Primitive>> _primitives;

        /**
         * Information of each primitive in the tree.
         * */
        struct BVHPrimitiveInfo {
            size_t prim_number;
            Bounds3 bounds;
            Point3 centroid;

            BVHPrimitiveInfo() {/*empty*/}

            BVHPrimitiveInfo(size_t prim_number, const Bounds3& bounds)
            : prim_number {prim_number}, bounds {bounds},
                centroid {.5f * bounds.pMin + .5f * bounds.pMax}
            {/*empty*/}
        };

        /**
         * O node in the BVH tree.
         * */
        struct BVHBuildNode {
            Bounds3 bounds;
            BVHBuildNode *children[2];
            int split_axis, first_prim_offset, n_primitives;

            void init_leaf(int first, int n, const Bounds3& b) {
                first_prim_offset = first;
                n_primitives = n;
                bounds = b;
                children[0] = children[1] = nullptr;
            }

            void init_interior(int axis, BVHBuildNode* c0, BVHBuildNode* c1) {
                children[0] = c0;
                children[1] = c1;
                bounds = c0->bounds.get_union(c0->bounds, c1->bounds);
                split_axis = axis;
                n_primitives = 0;
            }


        };

    public:

        BVHAccel(const std::vector<std::shared_ptr<Primitive>>& primitives,
                int max_prims_node, SplitMethod split_method);

        BVHBuildNode* recursive_build(std::vector<BVHPrimitiveInfo>& prim_info, 
                int start, int end, int* total_nodes,
                std::vector<std::shared_ptr<Primitive>>& ordered_prims);

        virtual bool intersect(const Ray& ray, SurfaceInteraction* interaction) const;

        virtual bool intersect(const Ray& ray) const;

        virtual const Material* get_material() const {
            return nullptr;
        }

        Bounds3 world_bound() const;

};
};
#endif
