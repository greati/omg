#ifndef __PARSER__
#define __PARSER__

#include <memory>
#include "scene/Scene.h"
#include "exceptions.h"

namespace rayt {
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
        virtual std::shared_ptr<Scene> parse_text(const std::string & text) = 0;

        /**
         * Parse a text from a file.
         *
         * @param file_path the file path
         * */
        virtual std::shared_ptr<Scene> parse_file(const std::string & file_path) = 0; 

};
};
#endif

