#ifndef _FLAT_MATERIAL_
#define _FLAT_MATERIAL_

#include "omg/materials/Material.h"
#include "omg/common.h"

namespace omg {
/**
 * Define an material for flat objects.
 *
 * @author Vitor Greati
 * */
class FlatMaterial : public Material {

    private:

        RGBColor _color;        /** The color of the material */

    public:

        FlatMaterial(const RGBColor& color) : _color {color} {}

        inline RGBColor kd() const { return _color; }

};
};
#endif
