#ifndef _DEPTH_INT_
#define _DEPTH_INT_

#include "omg/integrators/SamplerIntegrator.h"
#include "omg/common.h"
#include <limits>

namespace omg {
/**
 * An integrator that only returns a solid color
 * for a object.
 *
 * @author Vitor Greati
 * */
class DepthIntegrator : public SamplerIntegrator {

    private:

        float tMax = std::numeric_limits<float>::min();
        float tMin = 1.0;

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
            SurfaceInteraction* si;
            if (scene.intersect(ray, si)) {
                auto t = si->_t;
                float t_norm = (t - tMin)/(tMax - tMin); 
                float chann_t = 255 - t_norm * 255.0;
                return RGBColor {chann_t, chann_t, chann_t};
            } else {
                return scene.get_background()->find(px, py);
            }
        }

        void preprocess(const Scene& scene, Sampler* sampler) override {
            auto camera = scene.get_camera();

            this->_camera = camera;

            auto width = camera->get_width();
            auto height = camera->get_height();
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    auto [px, py] = std::pair{x / static_cast<float>(width), y / static_cast<float>(height)};
                    Ray ray = camera->generate_ray(px, py);
                    SurfaceInteraction* si;
                    if (scene.intersect(ray, si)) {
                        tMax = std::max(tMax, si->_t); 
                    }
                }
            }
        }

};
};
#endif
