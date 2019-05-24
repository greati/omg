#ifndef _BLINN_MATERIAL_
#define _BLINN_MATERIAL_

#include "omg/materials/Material.h"
#include "omg/common.h"

namespace omg {
/**
 * Define an material for Blinn Phong integrator.
 *
 * @author Vitor Greati
 * */
class BlinnMaterial : public Material {

    private:

        Vec3 _ka;       /** Ambient constants */
        Vec3 _kd;       /** Diffuse constants */
        Vec3 _ks;       /** Specular constants */
        Vec3 _km;       /** Mirror constants */
        RealValue _glossiness;

    public:

        BlinnMaterial(
                const decltype(_ka)& _ka,
                const decltype(_kd)& _kd,
                const decltype(_ks)& _ks,
                const decltype(_km)& _km,
                const decltype(_glossiness)& _glossiness
                ) : Material{}, _ka {_ka}, _kd {_kd}, _ks {_ks}, _km {_km}, _glossiness {_glossiness} {}

        ~BlinnMaterial() {}

        inline const decltype(_ka)& ka() const { return _ka; }
        inline const decltype(_kd)& kd() const { return _kd; }
        inline const decltype(_ks)& ks() const { return _ks; }
        inline const decltype(_ks)& km() const { return _km; }
        inline decltype(_glossiness) glossiness() const { return _glossiness; }
};
};
#endif
