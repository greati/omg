#ifndef _OMG_TRANS_CACHE_
#define _OMG_TRANS_CACHE_

#include "omg/objects/Transform.h"
#include <optional>

namespace omg {
/**
 * Cache of transforms.
 *
 * @author Vitor Greati
 * */
class TransformCache {

    private:

        std::map<
            Transform, 
            std::pair<
                std::shared_ptr<Transform>, 
                std::shared_ptr<Transform>
            >
        > cache;

    public:



        inline bool lookup(const Transform& t, 
                std::shared_ptr<Transform>& m_dest,
                std::shared_ptr<Transform>& m_inv_dest) {
            auto it = cache.find(t);
            if (it != cache.end()) {
               m_dest = it->second.first;
               m_inv_dest = it->second.second;
               return true;
            } else {
               auto mat = std::make_shared<Transform>(t);
               auto it_insert = cache.insert(std::pair(
                                *mat,
                                std::pair(
                                    mat,
                                    std::make_shared<Transform>(inverse(*mat))
                                )
                           )); 
               m_dest = it_insert.first->second.first;
               m_inv_dest = it_insert.first->second.second;
               return false;
            }
        }

        inline bool lookup(const Transform& t, 
                std::shared_ptr<Transform>& m_dest) {
            auto it = cache.find(t);
            if (it != cache.end()) {
               m_dest = it->second.first;
               return true;
            } else {
               auto mat = std::make_shared<Transform>(t);
               auto it_insert = cache.insert(std::pair(
                                *mat,
                                std::pair(
                                    mat,
                                    std::make_shared<Transform>(inverse(*mat))
                                )
                           )); 
               m_dest = it_insert.first->second.first;
               return false;
            }
        }
};
};

#endif
