#include <iostream>
#include "printer/NetpbmPrinter.h"
#include "omg/parsing/yaml/YAMLParser.h"
#include "omg/raytracer/RaytracerVisitor.h"
#include "omg/integrators/SamplerIntegrator.h"
#include "omg/integrators/FlatIntegrator.h"
#include "omg/integrators/DepthIntegrator.h"
#include "omg/integrators/NormalMapIntegrator.h"
#include "omg/raytracer/RaytracerEngine.h"

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

    omg::RaytracerEngine engine;

    try {
        engine.run(file_path, destination);
    } catch (omg::ParseException& e) {
        omg::logger.log(Logger::Type::ERROR, "parsing", e.message());
    }

    return 0;
}
