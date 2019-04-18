#ifndef _FLAT_INT_
#define _FLAT_INT_

#include "omg/integrators/SamplerIntegrator.h"
#include "omg/common.h"

namespace omg {
/**
 * An integrator that only returns a solid color
 * for a object.
 *
 * @author Vitor Greati
 * */
class FlatIntegrator : public SamplerIntegrator {

    public:

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
            bool hit = scene.intersect(ray);
            return hit
                ? RGBColor {255, 0, 0}
            : scene.get_background()->find(px, py);
        }

};
};
#endif
