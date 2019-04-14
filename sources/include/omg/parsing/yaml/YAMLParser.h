#ifndef __YAMLPARSER__
#define __YAMLPARSER__

#include "omg/parsing/Parser.h"
#include "yaml-cpp/yaml.h"

namespace omg {
/**
 * A scene parser using YAML.
 *
 * @author Vitor Greati
 * */
class YAMLParser : public Parser {

    public:

        std::shared_ptr<Scene> parse_text(const std::string & text) override;

        std::shared_ptr<Scene> parse_file(const std::string & file_path) override; 

    private:

        /**
         * Parse some node of a specific type.
         * */
        template<typename NodeType>
        std::shared_ptr<NodeType> parse(const YAML::Node& node); 

        /**
         * Parse a list of nodes.
         * */
        template<typename NodeType>
        std::vector<std::shared_ptr<NodeType>> parse_list(const YAML::Node& node);

        /**
         * Check if a node exists in the current one. If not, throws
         * an error.
         *
         * @param curr_node the current node
         * @param node_name the name of the node
         * @return if the node exists in the current one
         * */
        YAML::Node hard_require(const YAML::Node & curr_node, const std::string& node_name) const;

        /**
         * Check if a node exists in the current one. If not, print a warning.
         *
         * @param curr_node the current node
         * @param node_name the name of the node
         * @return if the node exists in the current one
         * */
        bool soft_require(const YAML::Node & curr_node, const std::string& node_name) const;

};
};
#endif
