#ifndef __SCENE__
#define __SCENE__

#include "SceneNode.h"
#include "omg/cameras/Camera.h"
#include "omg/backgrounds/Background.h"

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

    public:

        /**
         * Empty constructor.
         * */
        Scene() { /* empty */ }

        /**
         * Complete scene constructor.
         * */
        Scene(std::shared_ptr<Background> background,
              std::shared_ptr<Camera> camera)
            : _background {background},
              _camera {camera}
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

        void accept(Visitor& visitor) override {
            visitor.visit(std::shared_ptr<Scene>(this));
        };

};
};
#endif