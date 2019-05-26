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

        int _spp;                            /** samples per pixel */

        std::shared_ptr<Sampler> _sampler;

    public:

        SamplerIntegrator(int spp = 1) : _spp {spp} {}

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

            this->preprocess(scene, _sampler.get());

            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    //TODO include sampler
                    auto [px, py] = std::pair{x / static_cast<float>(width), y / static_cast<float>(height)};
                    Ray ray = camera->generate_ray(px, py);
                    RGBColor color = this->li(ray, scene);
                    camera->get_film()->get_buffer()->set({x, y}, {
                            static_cast<unsigned char>(color(0)), 
                            static_cast<unsigned char>(color(1)), 
                            static_cast<unsigned char>(color(2))});
                }
            }
        }


};
};
#endif
