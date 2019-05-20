#ifndef __SCENE__
#define __SCENE__

#include "omg/cameras/Camera.h"
#include "omg/backgrounds/Background.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/objects/Primitive.h"
#include "omg/lights/Light.h"

namespace omg {
/**
 * Represents a  3D scene.
 *
 * @author Vitor Greati
 * */
class Scene {

    private:

        std::shared_ptr<Background> _background;        /** Background reference */
        std::shared_ptr<Camera> _camera;                /** Camera reference */
        std::vector<std::shared_ptr<Primitive>> _primitives;  /** Scene objects */
        std::vector<std::shared_ptr<Light>> _lights;

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
              const decltype(_primitives)& primitives,
              const decltype(_lights)& lights)
            : _background {background},
              _camera {camera},
              _primitives {primitives},
              _lights {lights}
        { /* empty */ }

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

        inline const std::vector<std::shared_ptr<Primitive>>& get_primitives() const { return _primitives; }

        inline const std::vector<std::shared_ptr<Light>>& get_lights() const { return _lights; }

        //void accept(Visitor& visitor) override {
        //    visitor.visit(std::shared_ptr<Scene>(this));
        //};

        bool intersect(const Ray& ray, SurfaceInteraction* si) const;

        bool intersect(const Ray& ray) const;
        

};
};
#endif
