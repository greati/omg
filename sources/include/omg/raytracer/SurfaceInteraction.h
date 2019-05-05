#ifndef _SURF_INTER_
#define _SURF_INTER_

#include "omg/common.h"
#include "omg/objects/Primitive.h"

namespace omg {

class Primitive;

/**
 * Data regarding interaction of rays
 * with surfaces.
 *
 * @author Vitor Greati
 * */
class SurfaceInteraction {

    public:

        Point3 _p;                                          /** Contact point */
        Vec3 _n;                                            /** The surface normal */
        Vec3 _wo;                                           /** Outgoing direction of light, -ray*/
        float _time;                                        /** Time of contact */
        Point2 _uv;                                         /** Parametric coordinate of hit surface */
        float _t;                                           /** Ray t parameter for which intersection occurred */
        Primitive *_primitive = nullptr;    /** Pointer to the object */

    public:

        SurfaceInteraction() {/*empty*/}
        
        SurfaceInteraction(const Point3& p, const Vec3& n,
                const Vec3& wo, float time, const Point2& uv,
                float t,
                Primitive* primitive) 
        : _p {p}, _n {n}, _wo {wo}, _time {time}, _uv {uv}, _t {t}, _primitive {primitive}
        {/*empty*/}

};
};
#endif
