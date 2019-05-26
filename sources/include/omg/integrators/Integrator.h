#ifndef _OMG_INTEGRATOR_
#define _OMG_INTEGRATOR_

#include "omg/samplers/Sampler.h"

namespace omg {
/**
 * Implements the rendering algorithm.
 *
 * @author Vitor Greati
 * */
class Integrator {

    protected:

        std::shared_ptr<Camera> _camera;
        std::string _suffix = "";

    public:

        /**
         * Render a given scene.
         *
         * @param scene the scene to be rendered
         * */
        virtual void render(const Scene& scene) = 0;

        /**
         * Incidence radiance computation for a given ray
         * acting on a scene.
         *
         * @param ray the ray
         * @param scene the scene
         * @param sampler pointer to a sampler
         * */
        virtual RGBColor li(
                const Ray& ray,
                const Scene& scene,
                const std::shared_ptr<Sampler> sampler = nullptr,
                int depth=0) = 0;

        /**
         * Execute after parsing the scene, to do specific
         * implementations.
         *
         * @param scene the scene
         * @param sampler a sampler
         * */
        virtual void preprocess(const Scene& scene, Sampler* sampler) { }

        inline Camera* get_camera() const { return _camera.get(); }

        inline void set_suffix(const std::string & suff) { this->_suffix = suff; }

        inline std::string get_suffix() { return this->_suffix; }

};
};
#endif
