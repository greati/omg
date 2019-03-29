#ifndef __SCENE__
#define __SCENE__

#include "SceneNode.h"
#include "backgrounds/Background.h"

namespace rayt {
/**
 * Represents a  3D scene.
 *
 * @author Vitor Greati
 * */
class Scene : public SceneNode {

    private:

        std::shared_ptr<Background> _background;     /** Background reference */
        int _width {0};                              /** Scene width */
        int _height {0};                             /** Scene height */

    public:

        /**
         * Empty constructor.
         * */
        Scene() { /* empty */ }

        /**
         * Complete scene constructor.
         * */
        Scene(std::shared_ptr<Background> background, 
              int width,
              int height) 
            : _background {background}, 
              _width {width}, 
              _height {height}
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
            
                inline void set_height(int height) { _result->set_height(height); }

                inline void set_width(int width) { _result->set_width(width); }

                inline void set_background(std::shared_ptr<Background> background) { 
                    _result->set_background(background); 
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
        std::shared_ptr<Background> get_background() const { return this->_background; }

        /**
         * The scene width
         *
         * @return the scene width
         * */
        int get_width() const { return this->_width; }

        /**
         * The scene height
         *
         * @return the scene height
         * */
        int get_height() const { return this->_height; }

        /**
         * Set the scene width
         *
         * @param width the scene width
         * */
        void set_width(int width) { this->_width = width; }

        /**
         * Set the scene height
         *
         * @param width the scene height
         * */
        void set_height(int height) { this->_height = height; }

        /**
         * Set the scene background
         *
         * @param width the scene background
         * */
        void set_background(std::shared_ptr<Background> background) { this->_background = background; }

        void accept(Visitor& visitor) override {
            visitor.visit(std::shared_ptr<Scene>(this));
        };

};
};
#endif
