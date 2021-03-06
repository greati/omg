#ifndef __SCENE__
#define __SCENE__

#include "omg/cameras/Camera.h"
#include "omg/backgrounds/Background.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/objects/Primitive.h"
#include "omg/objects/AggregatePrimitive.h"
#include "omg/lights/Light.h"

namespace omg {

class Bounds3;
/**
 * Represents a  3D scene.
 *
 * @author Vitor Greati
 * */
class Scene {

    private:

        std::shared_ptr<Background> _background;        /** Background reference */
        std::shared_ptr<Camera> _camera;                /** Camera reference */
        std::vector<std::shared_ptr<Light>> _lights;

        std::shared_ptr<AggregatePrimitive> _aggregate;

        Bounds3 _world_bound;

    public:

        /**
         * Empty constructor.
         * */
        Scene() { /* empty */ }

        /**
         * Complete scene constructor.
         * */
        Scene(std::shared_ptr<Background> background,
              std::shared_ptr<Camera> camera,
              const decltype(_aggregate)& aggregate,
              const decltype(_lights)& lights)
            : _background {background},
              _camera {camera},
              _aggregate {aggregate},
              _lights {lights}
        {

            //> Compute world bound from primitive
            _world_bound = _aggregate->world_bound();

            //> Call scene preprocessing
            for (auto& l : _lights)
                l->preprocess(*this);
        }

        /**
         * The scene background.
         *
         * @return a pointer to the background
         * */
        inline std::shared_ptr<Background> get_background() const { return this->_background; }

        /**
         * Set the scene background
         *
         * @param width the scene background
         * */
        inline void set_background(std::shared_ptr<Background> background) { this->_background = background; }

        /**
         * Retrieve the camera
         *
         * @return the camera
         * */
        inline std::shared_ptr<Camera> get_camera() const { return this->_camera; }

        /**
         * Set the scene camera
         *
         * @param camera the camera
         * */
        inline void set_camera(std::shared_ptr<Camera> camera) { this->_camera = camera; }

        inline const std::vector<std::shared_ptr<Light>>& get_lights() const { return _lights; }

        bool intersect(const Ray& ray, SurfaceInteraction* si) const;

        bool intersect(const Ray& ray) const;

        inline const Bounds3& world_bound() const { return _world_bound; }
        

};
};
#endif
