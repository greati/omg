#include "omg/accelerators/BVHAccel.h"

using namespace omg;

BVHAccel::BVHAccel(const std::vector<std::shared_ptr<Primitive>>& primitives,
                int max_prims_node, SplitMethod split_method)
    : _primitives {primitives}, 
      _max_prims_node {std::min(255, max_prims_node)}, 
      _split_method {split_method}{

    if (primitives.size() == 0)
        return;

    //> Compute the tree
    //- Initialize vector of infos
    std::vector<BVHPrimitiveInfo> prim_infos {_primitives.size()};
    for (size_t i {0}; i < primitives.size(); ++i) {
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

    print_tree(root);
    //# print ordered prims
    std::cout << total_nodes << std::endl;
    std::cout << ordered_prims.size() << std::endl;

    _primitives.swap(ordered_prims);
    //- Flatten the tree
    nodes.reset(new LinearBVHNode[total_nodes]);
    int offset = 0;
    this->flatten_bvh_tree(root, &offset);

    //# print nodes
    for (int i = 0; i < _primitives.size(); ++i) {
        std::cout << nodes[i].n_primitives << std::endl;
    }
}

void BVHAccel::print_tree(BVHBuildNode* root) const {
    if (root->children[0] == nullptr && root->children[1] == nullptr) {
        std::cout << "l: " << root->n_primitives << std::endl;         
    } else {
        std::cout << "i: " << root->n_primitives << std::endl;         
        print_tree(root->children[0]);
        print_tree(root->children[1]);
    }
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
    // if 1 primitive in the box, make a leaf
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

int BVHAccel::flatten_bvh_tree(BVHBuildNode* node, int *offset) {
    LinearBVHNode* linear_node = &nodes[*offset];
    linear_node->bounds = node->bounds;
    int my_offset = (*offset)++;
    if (node->n_primitives > 0) {
        linear_node->primitives_offset = node->first_prim_offset;
        linear_node->n_primitives = node->n_primitives;
    } else {
        linear_node->axis = node->split_axis;
        linear_node->n_primitives = 0;
        flatten_bvh_tree(node->children[0], offset);
        linear_node->secondChild_offset = flatten_bvh_tree(node->children[1], offset);       
    }
    return my_offset;
}

bool BVHAccel::intersect(const Ray& ray, SurfaceInteraction* interaction) const {
    bool hit = false;
    const auto& r_dir = ray.get_direction();
    Vec3 inv_dir ({1/r_dir(0), 1/r_dir(1), 1/r_dir(2)});
    int dir_is_neg[3] = {inv_dir(0) < 0, inv_dir(1) < 0, inv_dir(2) < 0};
    
    int to_visit_offset = 0, current_node_index = 0;
    int nodes_to_vis[64];
    while (true) {
        const LinearBVHNode* node = &nodes[current_node_index];
        if (node->bounds.intersect(ray, inv_dir, dir_is_neg)) {
            if (node->n_primitives > 0) {
                for (int i = 0; i < node->n_primitives; ++i) {
                    if (_primitives[node->primitives_offset + i]->intersect(ray, interaction)) {
                        hit = true;
                    }
                }    
                if (to_visit_offset == 0) break;
                current_node_index = nodes_to_vis[--to_visit_offset];
            } else {
                if (dir_is_neg[node->axis]) {
                    nodes_to_vis[to_visit_offset++] = current_node_index + 1;
                    current_node_index = node->secondChild_offset;
                } else {
                    nodes_to_vis[to_visit_offset++] = node->secondChild_offset;
                    current_node_index = current_node_index + 1;
                }
            }
        } else {
            if (to_visit_offset == 0) break; 
            current_node_index = nodes_to_vis[--to_visit_offset];
        }
    }

    return hit;
}

bool BVHAccel::intersect(const Ray& ray) const {
    bool hit = false;
    const auto& r_dir = ray.get_direction();
    Vec3 inv_dir ({1/r_dir(0), 1/r_dir(1), 1/r_dir(2)});
    int dir_is_neg[3] = {inv_dir(0) < 0, inv_dir(1) < 0, inv_dir(2) < 0};
    
    int to_visit_offset = 0, current_node_index = 0;
    int nodes_to_vis[64];
    while (true) {
        const LinearBVHNode* node = &nodes[current_node_index];
        if (node->bounds.intersect(ray, inv_dir, dir_is_neg)) {
            if (node->n_primitives > 0) {
                for (int i = 0; i < node->n_primitives; ++i) {
                    if (_primitives[node->primitives_offset + i]->intersect(ray)) {
                        return true;
                    }
                }    
                if (to_visit_offset == 0) break;
                current_node_index = nodes_to_vis[--to_visit_offset];
            } else {
                if (dir_is_neg[node->axis]) {
                    nodes_to_vis[to_visit_offset++] = current_node_index + 1;
                    current_node_index = node->secondChild_offset;
                } else {
                    nodes_to_vis[to_visit_offset++] = node->secondChild_offset;
                    current_node_index = current_node_index + 1;
                }
            }
        } else {
            if (to_visit_offset == 0) break; 
            current_node_index = nodes_to_vis[--to_visit_offset];
        }
    }

    return hit;
}

Bounds3 BVHAccel::world_bound() const {
    return nodes[0].bounds;
}


