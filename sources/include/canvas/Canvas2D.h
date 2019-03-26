#ifndef __CANVAS2D__
#define __CANVAS2D__

#include <memory>
#include "Canvas.h"
#include "common.h"

using namespace rayt;

/**
 * A canvas for drawing 2D scenes.
 *
 * @author Vitor Greati
 */
template<int N>
class Canvas2D : public Canvas<Point2, N> {

    private:
        int _width;                                  /* Canvas width */
        int _height;                                 /* Canvas height */
        std::unique_ptr<unsigned char[]> _data;        /* Canvas pixels */

    public:
        
        /**
         * Constructor based on scene size.
         *
         * @param width the scene width
         * @param height the scene height
         * */
        Canvas2D(int width, int height);

        PixelValue<N> at(Point2 point) const override;

        void set(Point2 point, PixelValue<N> value) const override;

        void set(Point2 point, PixelValue<N> value, std::nothrow_t) const noexcept override;

        /**
         * The number of channels in the canvas.
         *
         * @return the number of channels
         * */
        inline int channels() const noexcept { return N; }

        inline int width() const noexcept override { return _width; }

        inline int height() const noexcept override { return _height; }

        void reset(Size<2> size) override;

        void clear(const PixelValue<N> & value) override;

        /**
         * A pointer to the canvas data.
         *
         * @return a pointer to the canvas data
         * */
        inline unsigned char* data() noexcept { return _data.get(); }

    private:
    
        /**
         * Validates a given point with respect to the width and height range.
         *
         * @param point the point
         * */
        void validate(const Point2 & point) const;
        
        /**
         * Given a point, return its real position inside the data member.
         *
         * @param point the point
         * @return the real position in the data member
         * */
        std::pair<int, int> real_pos(const Point2 & point) const;
        
};

#endif

