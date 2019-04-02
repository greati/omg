#include "omg/parsing/yaml/YAMLParser.h"
#include "omg/backgrounds/GradBilinearBackground.h"
#include "omg/backgrounds/SolidBackground.h"
#include <iostream>

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
        return true;
    }
};

template<>
std::shared_ptr<Background> YAMLParser::parse(const YAML::Node& node) {
    try {
        auto background_node = hard_require(node, "background");

        auto background_type = hard_require(background_node, "type").as<std::string>();

        if (background_type == "gradient") {
            auto colors = hard_require(background_node, "colors").as<std::vector<RGBColor>>();
            if (colors.size() > 4)
                omg::logger.log(Logger::Type::WARNING, "parsing", "only first four gradient colors were considered");
            return std::make_shared<GradBilinearBackground>(colors);
        } else if (background_type == "solid") {
            auto color = hard_require(background_node, "color").as<RGBColor>(); 
            return std::make_shared<SolidBackground>(color); 
        } else {
            return nullptr;
        }
    } catch (omg::ParseException & e) {
        throw;
    } catch (YAML::BadConversion & e) {
        throw omg::ParseException(e.what());
    }
}

template<>
std::shared_ptr<Camera> YAMLParser::parse(const YAML::Node& node) {
    try {
        auto camera_node = hard_require(node, "camera");
        int width = hard_require(camera_node, "width").as<int>();
        int height = hard_require(camera_node, "height").as<int>();
    
        if (camera_node["type"])
            omg::logger.log(Logger::Type::WARNING, "parsing", "type for camera not supported yet");

        return std::make_shared<Camera>(width, height);
    } catch (omg::ParseException & e) {
        throw;
    } catch (YAML::BadConversion & e) {
        throw omg::ParseException(e.what());
    }
    return nullptr;
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

YAML::Node YAMLParser::hard_require(const YAML::Node & curr_node, const std::string& node_name) const {
    if (!curr_node[node_name]) {
        throw ParseException("missing node " + node_name, curr_node.Mark().line, curr_node.Mark().column);
    }
    return curr_node[node_name];
}

bool YAMLParser::soft_require(const YAML::Node & curr_node, const std::string& node_name) const {
    return curr_node[node_name];
}
