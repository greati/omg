#ifndef __PARSER__
#define __PARSER__

#include <memory>
#include "omg/scene/Scene.h"
#include "omg/exceptions.h"
#include "omg/raytracer/RaytracerRunningSettings.h"

namespace omg {
/**
 * An interface for scene parsers.
 *
 * @author Vitor Greati
 * */
class Parser {

    public:

        /**
         * Parse a text.
         *
         * @param text the text to be parsed
         * */
        virtual void init_from_text(const std::string & text) = 0;

        /**
         * Parse a text from a file.
         *
         * @param file_path the file path
         * */
        virtual void init_from_file(const std::string & file_path) = 0; 

        /**
         * Parse a scene.
         * */
        virtual std::shared_ptr<Scene> parse_scene() = 0;

        /**
         * Parse the running settings.
         * */
        virtual std::shared_ptr<RaytracerRunningSettings> parse_running_settings() = 0;

};
};
#endif

