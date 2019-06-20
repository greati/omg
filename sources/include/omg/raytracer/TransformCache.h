#ifndef _OMG_TRANS_CACHE_
#define _OMG_TRANS_CACHE_

#include "omg/objects/Transform.h"

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
                Transform** m_dest, 
                Transform** m_inv_dest) {
            auto it = cache.find(t);
            if (it != cache.end()) {
               if (m_dest != nullptr)
                   *m_dest = it->second.first.get();
               if (m_inv_dest != nullptr)
                   *m_inv_dest = it->second.second.get();
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
               if (m_dest != nullptr)
                   *m_dest = it_insert.first->second.first.get();
               if (m_inv_dest != nullptr)
                   *m_inv_dest = it_insert.first->second.second.get();
               return false;
            }
        }
};
};

#endif
