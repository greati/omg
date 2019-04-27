#ifndef _RTRACER_ENGINE_
#define _RTRACER_ENGINE_

#include "omg/raytracer/RaytracerRunningSettings.h"

namespace omg {
/**
 * Raytracer facade.
 *
 * @author Vitor Greati
 * */
class RaytracerEngine {

    private:

        std::shared_ptr<Scene> _scene;

        std::shared_ptr<RaytracerRunningSettings> _running_settings;

        std::shared_ptr<Parser> _parser = std::make_shared<YAMLParser>();

    public:

        enum class SourceType {
            FROM_FILE = 0,
            FROM_TEXT = 1
        };

        void run(const std::string & source_text,
                const std::string& destination_path, 
                SourceType source = SourceType::FROM_FILE) {
            init(source_text, source);
            run_raytracer(destination_path);
            clean(); 
        }

        void init(const std::string & file_path, SourceType source) {
            if (source == SourceType::FROM_FILE)
                _parser->init_from_file(file_path);
            else if (source == SourceType::FROM_TEXT)
                _parser->init_from_file(file_path);
            else 
                _parser->init_from_file(file_path);
            _scene = _parser->parse_scene();
            _running_settings = _parser->parse_running_settings();
        }

        void run_raytracer(const std::string& destination_path) {

            auto w = _scene->get_camera()->get_width();
            auto h = _scene->get_camera()->get_height();
            Configs<std::string> configs {{
                {netpbm::Options::IMAGE_WIDTH, w},
                {netpbm::Options::IMAGE_HEIGHT, h},
                {netpbm::Options::IMAGE_CHANNELS, 3},
                {netpbm::Options::PBM_TYPE, (int) netpbm::NetpbmType::PIX_MAP},
                {netpbm::Options::PBM_ENCODING, (int) netpbm::NetpbmEncoding::ASCII},
                {netpbm::Options::MAX_INTENSITY, 255}
            }};

            auto ppm_printer = std::make_shared<netpbm::NetpbmPrinter<unsigned char>>(configs);

            for (auto & integrator : _running_settings->integrators) {
                integrator->render(*_scene);
                integrator->get_camera()->get_film()->write(
                        destination_path + integrator->get_suffix(), {ppm_printer}); 
            }
        }

        void clean() {
            // empty for now
        }

};
};
#endif