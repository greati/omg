#ifndef _OMG_FILM_
#define _OMG_FILM_

#include "omg/common.h"
#include "omg/buffer/Buffer.h"
#include "printer/Printer.h"

/**
 * Film where the image is stored.
 *
 * @author Vitor Greati
 * */
namespace omg {
class Film {

    private:

        Point2i _full_resolution;                                              /** The image full resolution */
        std::unique_ptr<Buffer<3>> _buffer;                                    /** Raytracer buffer */

    public:

        Film(const Point2i & full_resolution) 
            : _full_resolution {full_resolution} {
            auto [w, h] = full_resolution;
            this->_buffer = std::move(std::make_unique<Buffer<3>>(w, h));
        }

        /**
         * Get the film data.
         *
         * @return the buffer
         * */
        inline Buffer<3>* get_buffer() { return _buffer.get(); }

        /**
         * Write image to file.
         *
         * */
        void write(const std::string& filename, 
                const std::vector<std::shared_ptr<Printer<unsigned char>>> & _printers = {}) {
            for (auto p : _printers)
                p->print(_buffer->data(), filename);
        }

        inline Point2i get_full_resolution() const { return _full_resolution; }

};
};
#endif

