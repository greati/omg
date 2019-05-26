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

        RGBColor _near_color;
        RGBColor _far_color;

        float _tMax = std::numeric_limits<float>::min();
        float _tMin = std::numeric_limits<float>::max();

        bool _fixed_t = false;

    public:

        DepthIntegrator(const RGBColor& near,
                const RGBColor& far, int spp = 1) 
            : SamplerIntegrator {spp}, _near_color{near}, _far_color{far} {
            this->_suffix = "_depth";
        }

        DepthIntegrator(const RGBColor& near,
                const RGBColor& far, float tMin, float tMax, int spp = 1) 
            : SamplerIntegrator {spp}, _near_color{near}, _far_color{far}, 
            _tMin {tMin}, _tMax {tMax}, _fixed_t {true} {
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
                const std::shared_ptr<Sampler> sampler = nullptr,
                int depth=0) override {
            SurfaceInteraction si;
            if (scene.intersect(ray, &si)) {
                auto t = si._t;
                float t_norm = t;
                float r = lerp(t_norm, _tMin, _near_color(0), _tMax, _far_color(0));
                float g = lerp(t_norm, _tMin, _near_color(1), _tMax, _far_color(1));
                float b = lerp(t_norm, _tMin, _near_color(2), _tMax, _far_color(2));
                return RGBColor{r, g, b};
            } else {
                return _far_color;
            }
        }

        float lerp(float x, float x0, float y0, float x1, float y1) const {
            return y0 + (y1-y0)/(x1-x0) * (x - x0);
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
                        _tMin = std::min(_tMin, si._t);
                    }
                }
            }
        }

};
};
#endif
