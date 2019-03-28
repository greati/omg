#ifndef __NODES__
#define __NODES__

#include "Visitor.h"

namespace rayt {
/**
 * A node on the scene description.
 *
 * @author Vitor Greati
 * */
class Node {

    public:

        /**
         * Executes a visitor method for this node.
         *
         * @param visitor the visitor
         * */
        virtual void accept(std::shared_ptr<Visitor> visitor) = 0;

};
};
#endif
