#ifndef __RAYTRACERVISITOR__
#define __RAYTRACERVISITOR__

#include "omg/parsing/Visitor.h"
#include "omg/buffer/Buffer.h"

namespace omg {
/**
 * Executes the main operations of the ray tracer
 * as a visitor of a parsed scene.
 *
 * @author Vitor Greati
 * */
class RaytracerVisitor : public Visitor {

    private:

        std::unique_ptr<Buffer<3>> _buffer;     /** Raytracer buffer */

    public:
        
        /**
         * Perform the raytracer algorithm.
         *
         * @param scene a pointer to the scene to be drawn
         * */
        void visit(const std::shared_ptr<Scene>& scene) override;

        /**
         * Raytracer buffer.
         *
         * @return a pointer to the raytracer buffer
         * */
        inline Buffer<3>* get_buffer() const { return _buffer.get(); }

};
};
#endif
