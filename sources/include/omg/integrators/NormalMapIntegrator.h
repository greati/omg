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
            SurfaceInteraction* si = new SurfaceInteraction();
            if (scene.intersect(ray, si)) {
                Vec3 normal = (si->_n).unit().value();
                auto r = (normal(0) + 1.0) / 2.0;
                auto g = (normal(1) + 1.0) / 2.0;
                auto b = (normal(2) + 1.0) / 2.0;
                return RGBColor {r * 255, g * 255, b * 255};
            } else {
                return scene.get_background()->find(px, py);
            }
        }
};
};
#endif
