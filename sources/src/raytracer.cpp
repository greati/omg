#include <iostream>
#include "printer/NetpbmPrinter.h"
#include "omg/parsing/yaml/YAMLParser.h"
#include "omg/raytracer/RaytracerVisitor.h"
#include "omg/integrators/SamplerIntegrator.h"
#include "omg/integrators/FlatIntegrator.h"
#include "omg/integrators/DepthIntegrator.h"

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
        omg::logger.log(Logger::Type::ERROR, "parsing", e.message());
        return 1;
    }

    omg::logger.log(Logger::Type::SUCCESS, "parsing", "scene was parsed successfully!");

    omg::logger.log(Logger::Type::INFO, "raytracer", "starting raytracer...");

    ///std::shared_ptr<omg::SamplerIntegrator> integrator = std::make_shared<omg::FlatIntegrator>();
    std::shared_ptr<omg::SamplerIntegrator> integrator = std::make_shared<omg::DepthIntegrator>();

    integrator->render(*scene);

    omg::logger.log(Logger::Type::SUCCESS, "raytracer", "scene was raytraced successfully!");

    omg::logger.log(Logger::Type::INFO, "saving", "saving the image...");

    auto [w, h] = integrator->get_camera()->get_film()->get_full_resolution();

    Configs<std::string> configs {{
        {netpbm::Options::IMAGE_WIDTH, w},
        {netpbm::Options::IMAGE_HEIGHT, h},
        {netpbm::Options::IMAGE_CHANNELS, 3},
        {netpbm::Options::PBM_TYPE, (int) netpbm::NetpbmType::PIX_MAP},
        {netpbm::Options::PBM_ENCODING, (int) netpbm::NetpbmEncoding::ASCII},
        {netpbm::Options::MAX_INTENSITY, 255}
    }};

    auto ppm_printer = std::make_shared<netpbm::NetpbmPrinter<unsigned char>>(configs);

    integrator->get_camera()->get_film()->get_printers().push_back(ppm_printer);

    integrator->get_camera()->get_film()->write(destination);

    omg::logger.log(Logger::Type::SUCCESS, "saving", "image saved successfully in " + destination);

    return 0;
}
