#ifndef _OMG_RT_RUNNING_SETT_
#define _OMG_RT_RUNNING_SETT_

#include "omg/integrators/Integrator.h"

/**
 * Running settings for the Raytracer.
 *
 * @author Vitor Greati
 * */
namespace omg {
struct RaytracerRunningSettings {

    public:

        std::vector<std::shared_ptr<Integrator>> integrators;

};
};
#endif

