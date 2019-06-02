#include "omg/accelerators/BVHAccel.h"

using namespace omg;

BVHAccel::BVHAccel(const std::vector<std::shared_ptr<Primitive>>& primitives,
                int max_prims_node, SplitMethod split_method)
    : _primitives {primitives}, 
      _max_prims_node {std::max(255, max_prims_node)}, 
      _split_method {split_method}{

    if (primitives.size() == 0)
        return;

    //> Compute the tree
    //- Initialize vector of infos
    std::vector<BVHPrimitiveInfo> prim_infos {_primitives.size()};
    for (size_t i {0}; i < prim_infos.size(); ++i) {
        prim_infos[i] = BVHPrimitiveInfo{i, _primitives[i]->world_bound()};
    }
    //- Call the appropriate method
    int total_nodes = 0;
    std::vector<std::shared_ptr<Primitive>> ordered_prims;
    BVHBuildNode* root;
    if (split_method == SplitMethod::HLBVH)
        throw std::invalid_argument("unavailable option");
    else
        root = recursive_build(prim_infos, 0, _primitives.size(), &total_nodes, ordered_prims);
    _primitives.swap(ordered_prims);
}


BVHAccel::BVHBuildNode* BVHAccel::recursive_build(std::vector<BVHPrimitiveInfo>& prim_info, 
        int start, int end, int* total_nodes,
        std::vector<std::shared_ptr<Primitive>>& ordered_prims) {

}

bool BVHAccel::intersect(const Ray& ray, SurfaceInteraction* interaction) const {
    return false;
}

bool BVHAccel::intersect(const Ray& ray) const {
    return false;
}

Bounds3 BVHAccel::world_bound() const {
    return Bounds3();
}


