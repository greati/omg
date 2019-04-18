#ifndef _OMG_SAMP_INTEGRATOR_
#define _OMG_SAMP_INTEGRATOR_

#include "omg/samplers/Sampler.h"
#include "omg/integrators/Integrator.h"

namespace omg {
/**
 * Integrator based on sampling.
 *
 * @author Vitor Greati
 * */
class SamplerIntegrator : public Integrator {

    protected:

        std::shared_ptr<Camera> _camera;

    public:

        /**
         * Render a given scene.
         *
         * @param scene the scene to be rendered
         * */
        void render(const Scene& scene) override {
            auto camera = scene.get_camera();

            this->_camera = camera;

            auto width = camera->get_width();
            auto height = camera->get_height();

            auto background = scene.get_background();

            camera->set_film(std::make_unique<Film>(Point2i{width, height}));

            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    auto [px, py] = std::pair{x / static_cast<float>(width), y / static_cast<float>(height)};
                    Ray ray = camera->generate_ray(px, py);
                    RGBColor color = this->li(ray, scene, px, py); // TODO: sampler will make px and py
                    auto [r, g, b] = std::tuple {color(0), color(1), color(2)};
                    camera->get_film()->get_buffer()->set({x, y}, {
                            static_cast<unsigned char>(r), 
                            static_cast<unsigned char>(g), 
                            static_cast<unsigned char>(b)});
                }
            }
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
            bool hit = scene.intersect(ray);
            return hit
                ? RGBColor {255, 0, 0}
            : scene.get_background()->find(px, py);
        }

        /**
         * Execute after parsing the scene, to do specific
         * implementations.
         *
         * @param scene the scene
         * @param sampler a sampler
         * */
        void preprocess(const Scene& scene, Sampler& sampler) override { 
        
        }

        inline Camera* get_camera() const { return _camera.get(); }

};
};
#endif
