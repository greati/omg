#ifndef _DEPTH_INT_
#define _DEPTH_INT_

#include "omg/integrators/SamplerIntegrator.h"
#include "omg/common.h"
#include <limits>

namespace omg {
/**
 * An integrator based on depth.
 *
 * @author Vitor Greati
 * */
class DepthIntegrator : public SamplerIntegrator {

    private:

        float _tMax = std::numeric_limits<float>::min();
        float _tMin = 1.0;

        bool _fixed_t = false;

    public:

        DepthIntegrator(int spp = 1) : SamplerIntegrator {spp} {
            this->_suffix = "_depth";
        }

        DepthIntegrator(float tMin, float tMax, int spp = 1) 
            : SamplerIntegrator {spp}, _tMin {tMin}, _tMax {tMax}, _fixed_t {true} {
            this->_suffix = "_depth";
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
            SurfaceInteraction si;
            if (scene.intersect(ray, &si)) {
                auto t = si._t;
                float t_norm = (t - _tMin)/(_tMax - _tMin); 
                float chann_t = 255 - t_norm * 255.0;
                return RGBColor {chann_t, chann_t, chann_t};
            } else {
                return scene.get_background()->find(px, py);
            }
        }

        void preprocess(const Scene& scene, Sampler* sampler) override {

            if (_fixed_t) return;

            auto camera = scene.get_camera();

            auto width = camera->get_width();
            auto height = camera->get_height();
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    auto [px, py] = std::pair{x / static_cast<float>(width), y / static_cast<float>(height)};
                    Ray ray = camera->generate_ray(px, py);
                    SurfaceInteraction si;
                    if (scene.intersect(ray, &si)) {
                        _tMax = std::max(_tMax, si._t); 
                    }
                }
            }
        }

};
};
#endif
