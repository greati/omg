#ifndef __VISITOR__
#define __VISITOR__

#include <memory>

namespace omg {

class Scene;

/**
 * Visitor class for omg scene descriptions.
 *
 * @author Vitor Greati
 * */
class Visitor {
    
    public:

        /**
         * Visit the scene root description.
         *
         * @param scene the scene object
         * */
        virtual void visit(const std::shared_ptr<Scene>& scene) = 0;
};
};
#endif
