#ifndef _OMG_SAMPLER_
#define _OMG_SAMPLER_

#include "omg/common.h"

namespace omg {
/**
 * Base sampler 
 *
 * */
class Sampler {


    private:

        int64_t _samples_per_pixel;             /** Samples per pixel */

    public:

        /**
         * Basic constructor.
         * */
        Sampler(int64_t samples_per_pixel) : _samples_per_pixel {samples_per_pixel} {}

        /**
         * Initialize the sampling for a given point,
         * used for performing set up operations
         * before the sampling algorithm.
         *
         * @param p the pixel
         * */
        virtual void start_pixel(const Point2& p) = 0;

        /**
         * Sample for the next dimension.
         *
         * @return the sample
         * */
        virtual float get_1d() = 0;

        /**
         * Sample for the next two dimensions.
         *
         * @return the sample
         * */
        virtual Point2 get_2d() = 0;

        /**
         * Sample an array of 1d samples.
         *
         * @param n number of elements
         * @return a pointer to a list of samples
         * */
        virtual std::unique_ptr<float> get_1d_array(int n);

        /**
         * Sample an array of 2d samples.
         *
         * @param n number of elements
         * @return a pointer to a list of samples
         * */
        virtual std::unique_ptr<Point2> get_2d_array(int n);

        /**
         * Method for starting the next sample.
         *
         * @return true until the number of the originally
         * requested samples per pixel have been generated.
         * */
        virtual bool start_next_sample();

        /**
         * To be used when concurrency is to be implemented,
         * so that each thread creates its own sampler.
         * 
         * @param seed the random seed that may be necessary for some samplers
         * @return a new sampler
         * */
        virtual std::unique_ptr<Sampler> clone(int seed = 1) = 0;

        /**
         * To be used when don't used all of the samples in a 
         * pixel before going to the next pixel, but instead
         * jump around pixels.
         *
         * @param sample_number the sampler sumber
         * @return false then sample_number is greater than the number of 
         * requested samples per pixel
         * */
        virtual bool set_sample_number(int64_t sample_number);
};
};
#endif
