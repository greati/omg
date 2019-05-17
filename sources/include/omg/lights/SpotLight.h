#ifndef _OMG_SPOT_LIGHT_
#define _OMG_SPOT_LIGHT_

#include "omg/common.h"

namespace omg {

/**
 * Represents a point light.
 *
 * @author Vitor Greati
 * */
class SpotLight : public Light {

    private:

        Point3 _position;
        Point3 _point_at;
        float _falloff;
        float _cutoff;
        float _cosFalloff;
        float _cosCutoff;

    public:

        SpotLight(const Vec3& intensity, const decltype(_position)& position,
                const decltype(_point_at)& point_at, float falloff, float cutoff)
            : Light {intensity}, _position {position}, _point_at {point_at}, _falloff {falloff},
                _cutoff {cutoff},
               _cosFalloff {std::cos(_falloff)},
              _cosCutoff {std::cos(_cutoff)} { /* empty */ }


        /**
         * Sample a point on the light source's surface.
         *
         * */
        virtual Vec3 sample_li(const SurfaceInteraction& interaction,
                Vec3 *wi) const {
            *wi = tao::unitize(_position - interaction._p);
            auto diff_norm = tao::norm(_position - interaction._p);
            auto diff_norm_squared = diff_norm * diff_norm;
            return _intensity * falloff(-*wi);// / diff_norm_squared;
        }

        float falloff(const Vec3 & w) const {
            Vec3 veclight = _point_at - _position;
            float cosTheta = tao::dot(veclight, w) / ((tao::norm(veclight) * tao::norm(w))); // z coord
            if (cosTheta < _cosCutoff) return 0;
            if (cosTheta > _cosFalloff) return 1;
            float delta = (cosTheta - _cosCutoff) / (_cosFalloff - _cosCutoff);
            return (delta * delta) * (delta * delta);
        }

        /**
         * Returns the direction of the light, when it depends
         * on the intersection position over the surface.
         *
         * @param position the intersection position
         * @return light direction
         * */
        virtual std::optional<Vec3> get_direction(const Vec3& surface_point) const {
            return Vec3{0, 0, 0};
        }

};

};

#endif
