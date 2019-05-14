#ifndef _BLINN_INT_
#define _BLINN_INT_

#include "omg/integrators/SamplerIntegrator.h"
#include "omg/materials/BlinnMaterial.h"
#include "omg/common.h"

namespace omg {
/**
 * Integrator based on Blinn Phong.
 *
 * @author Vitor Greati
 * */
class BlinnPhongIntegrator : public SamplerIntegrator {

    public:

        BlinnPhongIntegrator(int spp) : SamplerIntegrator(spp) {
            this->_suffix = "_blinn";
        }

        /**
         * Incidence radiance computation for a given ray
         * acting on a scene.
         *
         * @param ray the ray
         * @param scene the scene
         * @param sampler pointer to a sampler
         * */
        RGBColor li(const Ray& ray,
                const Scene& scene,
                float px = 0.0,
                float py = 0.0,
                const std::shared_ptr<Sampler> sampler = nullptr) override {
            return RGBColor{0, 0, 0};
        }

};
};
#endif
