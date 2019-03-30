#include "parsing/yaml/YAMLParser.h"

using namespace rayt;

template<>
std::shared_ptr<Background> YAMLParser::parse(const YAML::Node& node) {
    hard_require(node, "background");

    auto background_node = node["background"];


}

template<>
std::shared_ptr<Camera> YAMLParser::parse(const YAML::Node& node) {
    hard_require(node, "camera");

    auto camera_node = node["camera"];

    int width = camera_node["width"].as<int>();
    int height = camera_node["height"].as<int>();

    return std::make_shared<Camera>(width, height);
}

template<>
std::shared_ptr<Scene> YAMLParser::parse(const YAML::Node& node) {

    auto camera = this->parse<Camera>(node);
    auto background = this->parse<Background>(node);

    auto scene = std::make_shared<Scene>(background, camera);

    return scene;
}

std::shared_ptr<Scene> YAMLParser::parse_text(const std::string & text) {
    YAML::Node root = YAML::Load(text);
    return this->parse<Scene>(root);
}

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
