#include "parsing/yaml/YAMLParser.h"

using namespace rayt;

template<>
std::shared_ptr<Scene> YAMLParser::parse(const YAML::Node& node) {
    hard_require(node, "width");
    hard_require(node, "height");
}

std::shared_ptr<Scene> YAMLParser::parse_text(const std::string & text) {}

std::shared_ptr<Scene> YAMLParser::parse_file(const std::string & file_path) {
    YAML::Node root = YAML::LoadFile(file_path);
    return this->parse<Scene>(root);
}

void YAMLParser::hard_require(const YAML::Node & curr_node, const std::string& node_name) const {
    if (!curr_node[node_name])
        throw ParseException("missing something " + node_name);
}

bool YAMLParser::soft_require(const YAML::Node & curr_node, const std::string& node_name) const {
    return curr_node[node_name];
}
