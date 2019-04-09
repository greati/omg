#include "omg/parsing/yaml/YAMLParser.h"
#include "omg/backgrounds/GradBilinearBackground.h"
#include "omg/backgrounds/SolidBackground.h"
#include "omg/cameras/OrthoCamera.h"
#include "omg/cameras/PerspectiveCamera.h"
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
std::shared_ptr<Camera::VpDims> YAMLParser::parse(const YAML::Node& node) {
    if (!node["vpdims"]) {
        return nullptr;
    }
    auto vpdims = node["vpdims"].as<std::vector<float>>();
    if (vpdims.size() != 4)
        throw omg::ParseException("provide valid vpdims for your camera");
    return std::make_shared<Camera::VpDims>(vpdims[0], vpdims[1], vpdims[2], vpdims[3]);
}

template<>
std::shared_ptr<Camera> YAMLParser::parse(const YAML::Node& node) {
    try {
        auto camera_node = hard_require(node, "camera");
        auto camera_type = hard_require(camera_node, "type").as<std::string>();
        int width = hard_require(camera_node, "width").as<int>();
        int height = hard_require(camera_node, "height").as<int>();
        auto target = hard_require(camera_node, "target").as<RGBColor>();
        auto position = hard_require(camera_node, "position").as<RGBColor>();
        auto up = hard_require(camera_node, "up").as<RGBColor>();

        auto vpdims = parse<Camera::VpDims>(camera_node);

        if (camera_type == "orthographic") {
            if (vpdims == nullptr)
                throw omg::ParseException("provide vpdims for ortho camera");
            return std::make_shared<OrthoCamera>(width, height, 
                    position, target, up, *vpdims);
        } else if (camera_type == "perspective") {
            float fdistance = hard_require(camera_node, "fdist").as<float>();
            if (vpdims != nullptr)
                return std::make_shared<PerspectiveCamera>(width, height, position, target, up, *vpdims, fdistance);
            float vaov = hard_require(camera_node, "v_angle").as<float>();
            
            if (camera_node["aspect_ratio"]) {
                auto aspect_ratio = camera_node["aspect_ratio"].as<float>();
                return std::make_shared<PerspectiveCamera>(width, height, position, target, up, aspect_ratio, fdistance, vaov);
            }
            return std::make_shared<PerspectiveCamera>(width, height, position, target, up, fdistance, vaov);
        } else {
            throw omg::ParseException("undefined camera type " + camera_type); 
        }
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
