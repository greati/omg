#ifndef __IMAGEBACKGROUND__
#define __IMAGEBACKGROUND__

#include "omg/common.h"
#include "Background.h"
#include "lodepng.h"

#include <vector>

/**
 * Image background.
 *
 * @author Vitor Greati
 * */

namespace omg {
class ImageBackground : public Background {

    private:

        std::vector<unsigned char> image;
        unsigned width, height;

    public:

        ImageBackground (const std::string & image_path) {
             unsigned error = lodepng::decode(image, width, height, image_path);
             if (error) {
                 std::cout << "error when loading the background image...";
                 return;
             }
        }

        RGBColor find(float x, float y) const override {
            int index = y * width * 4 + 4 * x;
            return {image[index], image[index+1], image[index+2]};
        }

};
};

#endif
