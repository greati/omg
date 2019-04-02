#ifndef __SCENENODE__
#define __SCENENODE__

#include "omg/parsing/Visitor.h"

namespace omg {
/**
 * Represents a scene node that can be visited
 * by a visitor.
 *
 * @author Vitor Greati
 * */
class SceneNode {
    
    public:

        /**
         * Accept a visitor to process this node
         *
         * @param visitor the visitor
         * */
        virtual void accept(Visitor& visitor) = 0;

};
};
#endif
