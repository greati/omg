#include <iostream>
#include "backgrounds/GradBackground.h"
#include "backgrounds/SolidBackground.h"
#include "backgrounds/ImageBackground.h"
#include "canvas/Canvas2D.h"
#include "printer/NetpbmPrinter.h"

int main(int argn, char* args[]) {
    std::string file_path;
    if (argn < 2) {
        std::cout << "provide a description file path";
        return 0;
    }
    file_path = std::string(args[1]);

    std::string destination;
    if (argn < 3) {
        std::cout << "provide the destination path";
        return 0;
    }

    destination = std::string(args[2]);

   //rayt::GradBackground grad {
   //    {300, 300},
   //    {255, 0, 0},
   //    {0, 255, 0},
   //    {0, 0, 255},
   //    {255, 255, 255}
   //};

   //rayt::SolidBackground grad {
   //    {255, 0, 0}
   //};
   
   rayt::ImageBackground grad {
       file_path
   };

    auto [x1, y1, z1] = grad.find({0.0, 0.0});
    auto [x2, y2, z2] = grad.find({0.0, 299.0});
    auto [x3, y3, z3] = grad.find({299.0, 0.0});
    auto [x4, y4, z4] = grad.find({299.0, 299.0});

    Canvas2D<3> canvas {300, 300};

    for (int i = 0; i < 300; ++i) {
        for (int j = 0; j < 300; ++j) {
            auto [r, g, b] = grad.find({j, 300-i});
            canvas.set({i, j}, {r, g, b});
        }
    }

    NetpbmPrinter<unsigned char> printer;

    Configs<NetpbmParams> configs {{
        {NetpbmParams::IMAGE_WIDTH, canvas.width()},
        {NetpbmParams::IMAGE_HEIGHT, canvas.height()},
        {NetpbmParams::IMAGE_CHANNELS, canvas.channels()},
        {NetpbmParams::PBM_TYPE, (int) NetpbmType::PIX_MAP},
        {NetpbmParams::PBM_ENCODING, (int) NetpbmEncoding::ASCII},
        {NetpbmParams::MAX_INTENSITY, 255}
    }};

    printer.print(canvas.data(), configs, destination);

    return 0;
}
