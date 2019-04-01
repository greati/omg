#include <iostream>
#include "backgrounds/GradBilinearBackground.h"
#include "backgrounds/SolidBackground.h"
#include "backgrounds/ImageBackground.h"
#include "buffer/Buffer.h"
#include "printer/NetpbmPrinter.h"
#include "parsing/yaml/YAMLParser.h"
#include "raytracer/RaytracerVisitor.h"

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

    rayt::YAMLParser yaml_parser;

    auto scene = yaml_parser.parse_file(file_path);

    rayt::RaytracerVisitor raytracer;

    raytracer.visit(scene);

/*  rayt::GradBilinearBackground grad {
      {255, 0, 0},
      {0, 255, 0},
      {0, 0, 255},
      {255, 255, 255}
  };
*/
   //rayt::SolidBackground grad {
   //    {255, 0, 0}
   //};
   
  //rayt::ImageBackground grad {
  //    file_path
  //};
/*
    rayt::Buffer<3> buffer {300, 300};

    for (int x = 0; x < 300; ++x) {
        for (int y = 0; y < 300; ++y) {
            auto [r, g, b] = grad.find(x / 300.0, y / 300.0);
            buffer.set({x, y}, {r, g, b});
        }
    }
*/
    auto buffer = raytracer.get_buffer();

    NetpbmPrinter<unsigned char> printer;

    Configs<NetpbmParams> configs {{
        {NetpbmParams::IMAGE_WIDTH, buffer->width()},
        {NetpbmParams::IMAGE_HEIGHT, buffer->height()},
        {NetpbmParams::IMAGE_CHANNELS, buffer->channels()},
        {NetpbmParams::PBM_TYPE, (int) NetpbmType::PIX_MAP},
        {NetpbmParams::PBM_ENCODING, (int) NetpbmEncoding::ASCII},
        {NetpbmParams::MAX_INTENSITY, 255}
    }};

    printer.print(buffer->data(), configs, destination);

    return 0;
}
