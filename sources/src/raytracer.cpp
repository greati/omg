#include <iostream>
#include "printer/NetpbmPrinter.h"
#include "omg/parsing/yaml/YAMLParser.h"
#include "omg/raytracer/RaytracerVisitor.h"

int main(int argn, char* args[]) {

    omg::logger.log(Logger::Type::INFO, "raytracer", "welcome to the omg raytracer");

    std::string file_path;
    if (argn < 2) {
        omg::logger.log(Logger::Type::ERROR, "raytracer", "provide the description file path (.yml)");
        return 0;
    }
    file_path = std::string(args[1]);

    std::string destination;
    if (argn < 3) {
        omg::logger.log(Logger::Type::ERROR, "raytracer", "provide the destination path");
        return 0;
    }

    destination = std::string(args[2]);

    omg::YAMLParser yaml_parser;

    std::shared_ptr<omg::Scene> scene;

    omg::logger.log(Logger::Type::INFO, "parsing", "starting scene parser...");

    try {
        scene = yaml_parser.parse_file(file_path);
    } catch (omg::ParseException & e) {
        omg::logger.log(Logger::Type::ERROR, "parsing", e.get_info());
        return 1;
    }

    omg::logger.log(Logger::Type::SUCCESS, "parsing", "scene was parsed successfully!");

    omg::logger.log(Logger::Type::INFO, "raytracer", "starting raytracer...");

    omg::RaytracerVisitor raytracer;

    raytracer.visit(scene);

    omg::logger.log(Logger::Type::SUCCESS, "raytracer", "scene was raytraced successfully!");

    omg::logger.log(Logger::Type::INFO, "saving", "saving the image...");

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

    omg::logger.log(Logger::Type::SUCCESS, "saving", "image saved successfully in " + destination);

    return 0;
}
