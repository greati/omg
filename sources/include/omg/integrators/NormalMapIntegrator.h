#ifndef _NN_INT_
#define _NN_INT_

#include "omg/integrators/SamplerIntegrator.h"
#include "omg/common.h"
#include <limits>

namespace omg {
/**
 * Integrator that computes a color based on the
 * normal vector in the point of intersection
 * to the surface.
 *
 * @author Vitor Greati
 * */
class NormalMapIntegrator : public SamplerIntegrator {

    public:

        NormalMapIntegrator(int spp = 1) : SamplerIntegrator(spp) {
            this->_suffix = "_normalmap";
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
            auto si = std::make_unique<SurfaceInteraction>();
            if (scene.intersect(ray, si.get())) {
                Vec3 normal = tao::unitize(si->_n);
                auto r = (normal(0) + 1.0f) / 2.0f;
                auto g = (normal(1) + 1.0f) / 2.0f;
                auto b = (normal(2) + 1.0f) / 2.0f;
                return RGBColor {r * 255.0f, g * 255.0f, b * 255.0f};
            } else {
                auto [phi, theta] = ray.get_spherical_angles();
                auto px = phi * INV_2PI;
                auto py = theta * INV_PI;
                return scene.get_background()->find(1.0-px, 1.0-py);
            }
        }
};
};
#endif
