#ifndef __SCENE__
#define __SCENE__

#include "SceneNode.h"
#include "omg/cameras/Camera.h"
#include "omg/backgrounds/Background.h"
#include "omg/objects/Object.h"
#include "omg/raytracer/SurfaceInteraction.h"

namespace omg {
/**
 * Represents a  3D scene.
 *
 * @author Vitor Greati
 * */
class Scene : public SceneNode {

    private:

        std::shared_ptr<Background> _background;        /** Background reference */
        std::shared_ptr<Camera> _camera;                /** Camera reference */
        std::vector<std::shared_ptr<Object>> _objects;  /** Scene objects */

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
              const decltype(_objects)& objects)
            : _background {background},
              _camera {camera},
              _objects {objects}
        { /* empty */ }

        /**
         * A builder for building scenes.
         *
         * @author Vitor Greati
         * */
        class Builder {
            protected:
                std::unique_ptr<Scene> _result;

            private:

                Builder() {
                    _result = std::make_unique<Scene>();
                }
            
                inline void set_background(std::shared_ptr<Background> background) { 
                    _result->set_background(background); 
                }

                inline void set_camera(std::shared_ptr<Camera> camera) { 
                    _result->set_camera(camera); 
                }

                inline std::unique_ptr<Scene> build() {
                    return std::move(_result);
                }
        };

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

        inline const std::vector<std::shared_ptr<Object>>& get_objects() const { return _objects; }

        void accept(Visitor& visitor) override {
            visitor.visit(std::shared_ptr<Scene>(this));
        };

        bool intersect(const Ray& ray, SurfaceInteraction*& si) const {
            //TODO this will change to _objects->intersect(ray, si) when _objetcs become a tree
            float min_t = -1.0f;
            auto new_si = std::make_unique<SurfaceInteraction>();
            for (auto & o : _objects) {
               if (o->intersect(ray, new_si.get())) {
                    if (min_t == -1.0f || min_t > new_si->_t) {
                        min_t = new_si->_t;
                        si = new_si.get();
                    }
               }
            }
            return (min_t > -1.0f);
        }

        bool intersect(const Ray& ray) const {
            for (auto & o : _objects) 
                if (o->intersect(ray))
                    return true;
            return false;
        }
        

};
};
#endif
