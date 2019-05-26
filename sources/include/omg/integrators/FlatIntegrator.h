#ifndef _FLAT_INT_
#define _FLAT_INT_

#include "omg/integrators/SamplerIntegrator.h"
#include "omg/materials/FlatMaterial.h"
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

        FlatIntegrator(int spp) : SamplerIntegrator(spp) {
            this->_suffix = "_flat";
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
                const std::shared_ptr<Sampler> sampler = nullptr,
                int depth=0) override {
            SurfaceInteraction si;
            bool hit = scene.intersect(ray, &si);
            if (hit) {
                const FlatMaterial* flat_mat = dynamic_cast<const FlatMaterial*>(si._primitive->get_material());
                return flat_mat->kd();
            } else {
                auto [phi, theta] = ray.get_spherical_angles();
                auto px = phi * INV_2PI;
                auto py = theta * INV_PI;
                return scene.get_background()->find(px, py);
            }
        }

};
};
#endif
