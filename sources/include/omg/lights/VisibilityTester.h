#ifndef _OMG_VIS_TESTER_
#define _OMG_VIS_TESTER_

#include "omg/scene/Scene.h"

namespace omg {

/**
 * Used for checking occlusions.
 *
 * @author Vitor Greati
 * */
class VisibilityTester {

    private:
        
        SurfaceInteraction si0;
        SurfaceInteraction si1;

    public:

        VisibilityTester() {/*empty*/}

        VisibilityTester(const decltype(si0)& si0, const decltype(si1)& si1) 
            : si0 {si0}, si1 {si1} {/* empty */}

        inline const decltype(si0)& get_si0() const {return si0;}
        inline const decltype(si1)& get_si1() const {return si1;}

        /**
         * Spawns a shadow ray searching for
         * occlusion.
         *
         * @param scene the scene
         * @return if occlusion happened
         * */
        bool unoccluded(const Scene& scene) const;

};
};
#endif
