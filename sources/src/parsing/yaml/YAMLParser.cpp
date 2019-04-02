#include "omg/parsing/yaml/YAMLParser.h"
#include "omg/backgrounds/GradBilinearBackground.h"
#include "omg/backgrounds/SolidBackground.h"

using namespace omg;

template<>
struct YAML::convert<RGBColor> {
    static YAML::Node encode(const RGBColor& rhs) {
        YAML::Node node;
        return node;
    }
    static bool decode(const YAML::Node& node, RGBColor & rgbColor) {
        if (!node.IsSequence() || node.size() != 3)
            return false;

       rgbColor(0) = node[0].as<float>(); 
       rgbColor(1) = node[1].as<float>(); 
       rgbColor(2) = node[2].as<float>(); 
       //std::get<0>(rgbColor) = node[0].as<int>(); 
       //std::get<1>(rgbColor) = node[1].as<int>(); 
       //std::get<2>(rgbColor) = node[2].as<int>(); 
        return true;
    }
};

template<>
std::shared_ptr<Background> YAMLParser::parse(const YAML::Node& node) {
    hard_require(node, "background");
    auto background_node = node["background"];

    hard_require(background_node, "type");
    auto background_type = background_node["type"].as<std::string>();

    if (background_type == "gradient") {
        auto ulc = background_node["uleft"].as<RGBColor>();
        auto urc = background_node["uright"].as<RGBColor>();
        auto llc = background_node["lleft"].as<RGBColor>();
        auto lrc = background_node["lright"].as<RGBColor>();
        return std::make_shared<GradBilinearBackground>(ulc, urc, lrc, llc);
    } else if (background_type == "solid") {
        auto color = background_node["color"].as<RGBColor>(); 
        return std::make_shared<SolidBackground>(color); 
    } else {
        return nullptr;
    }
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
    hard_require(root, "raytracer");
    return this->parse<Scene>(root["raytracer"]);
}

void YAMLParser::hard_require(const YAML::Node & curr_node, const std::string& node_name) const {
    if (!curr_node[node_name])
        throw ParseException("missing something " + node_name);
}

bool YAMLParser::soft_require(const YAML::Node & curr_node, const std::string& node_name) const {
    return curr_node[node_name];
}
