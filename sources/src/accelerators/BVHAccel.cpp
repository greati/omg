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

    std::unique_ptr<BVHBuildNode> node = std::make_unique<BVHBuildNode>();
    (*total_nodes)++;

    Bounds3 bounds;
    for (auto i {start}; i < end; ++i)
        bounds = bounds.get_union(bounds, prim_info[i].bounds);

    int n_primitives = end - start;
    if (n_primitives == 1) {
        int first_prim_offset = ordered_prims.size();
        for (auto i {start}; i < end; ++i) {
            int prim_num = prim_info[i].prim_number;
            ordered_prims.push_back(_primitives[prim_num]);
        }
        node->init_leaf(first_prim_offset, n_primitives, bounds);
        return node.get();
    } else {

        Bounds3 centroid_bounds;
        for (auto i {start}; i < end; ++i) {
            centroid_bounds = centroid_bounds.get_union(centroid_bounds, prim_info[i].centroid);
        }
        int dim = centroid_bounds.maximum_extent();

        int mid = (start + end) / 2;
        if (centroid_bounds.pMax(dim) == centroid_bounds.pMin(dim)) {
            // leaf node
            int first_prim_offset = ordered_prims.size();
            for (auto i {start}; i < end; ++i) {
                int prim_num = prim_info[i].prim_number;
                ordered_prims.push_back(_primitives[prim_num]);
            }
            node->init_leaf(first_prim_offset, n_primitives, bounds);
            return node.get();
        } else {
            switch(_split_method) {
                case SplitMethod::Middle: {
                    float pmid = (centroid_bounds.pMin(dim) + centroid_bounds.pMax(dim)) / 2;
                    BVHPrimitiveInfo* midPtr =
                        std::partition(&prim_info[start], &prim_info[end - 1] + 1,
                                [dim, pmid](const auto& pi) {
                                    return pi.centroid(dim) < pmid; 
                                });
                    mid = midPtr - &prim_info[0];
                    if (mid != start && mid != end)
                        break;
                }
                case SplitMethod::EqualCounts:
                    mid = (start + end) / 2;
                    std::nth_element(&prim_info[start], &prim_info[mid], &prim_info[end-1]+1,
                            [dim](const BVHPrimitiveInfo& a, const BVHPrimitiveInfo& b) {
                                return a.centroid(dim) < b.centroid(dim);
                            });
                    break;
            }
            // partition
            node->init_interior(dim,
                    recursive_build(prim_info, start, mid, total_nodes, ordered_prims),
                    recursive_build(prim_info, mid, end, total_nodes, ordered_prims));
        }

    }
    
    return node.get();
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


