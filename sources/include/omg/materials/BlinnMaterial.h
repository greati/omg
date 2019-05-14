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

        Vec3 _ka;
        Vec3 _kd;
        Vec3 _ks;
        RealValue _glossiness;

    public:

        BlinnMaterial(
                const decltype(_ka)& _ka,
                const decltype(_kd)& _kd,
                const decltype(_ks)& _ks,
                const decltype(_glossiness)& _glossiness
                ) : Material{}, _ka {_ka}, _kd {_kd}, _ks {_ks}, _glossiness {_glossiness} {}

        ~BlinnMaterial() {}

        inline decltype(_ka) ka() const { return _ka; }
        inline decltype(_kd) kd() const { return _kd; }
        inline decltype(_ks) ks() const { return _ks; }
        inline decltype(_glossiness) glossiness() const { return _glossiness; }
};
};
#endif
