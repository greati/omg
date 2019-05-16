#include "omg/parsing/yaml/YAMLParser.h"
#include "omg/backgrounds/GradBilinearBackground.h"
#include "omg/backgrounds/SolidBackground.h"
#include "omg/cameras/OrthoCamera.h"
#include "omg/cameras/PerspectiveCamera.h"
#include "omg/objects/Sphere.h"
#include "omg/objects/Object.h"
#include "omg/integrators/Integrator.h"
#include "omg/integrators/FlatIntegrator.h"
#include "omg/integrators/NormalMapIntegrator.h"
#include "omg/integrators/DepthIntegrator.h"
#include "omg/integrators/BlinnPhongIntegrator.h"
#include "omg/materials/Material.h"
#include "omg/materials/FlatMaterial.h"
#include "omg/materials/BlinnMaterial.h"
#include "omg/objects/GeometricPrimitive.h"
#include "omg/lights/Light.h"
#include "omg/lights/PointLight.h"
#include "omg/lights/DirectionalLight.h"
#include "omg/lights/AmbientLight.h"
#include "omg/lights/SpotLight.h"
#include <iostream>

using namespace omg;

template<>
struct YAML::convert<Vec3> {
    static YAML::Node encode(const Vec3& rhs) {
        YAML::Node node;
        return node;
    }
    static bool decode(const YAML::Node& node, Vec3 & rgbColor) {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rgbColor(0) = node[0].as<float>(); 
        rgbColor(1) = node[1].as<float>(); 
        rgbColor(2) = node[2].as<float>(); 
        return true;
    }
};

template<>
Vec3 YAMLParser::defaulted_require(const YAML::Node & curr_node, 
        const std::string& node_name, const Vec3& def) const {
    if (curr_node[node_name])
        return curr_node[node_name].as<Vec3>();
    return def;
}

template<>
std::shared_ptr<Sphere> YAMLParser::parse(const YAML::Node& node) {
    auto radius = hard_require(node, "radius").as<float>();
    auto center = hard_require(node, "center").as<Vec3>();
    return std::make_shared<Sphere>(radius, center);
}

template<>
std::shared_ptr<Primitive> YAMLParser::parse(const YAML::Node& node) {
    auto type = hard_require(node, "type").as<std::string>();

    if (type == "sphere") {
        auto sphereprim = parse<Sphere>(node);
        auto material = get_material(hard_require(node, "material").as<std::string>());
        return std::make_shared<GeometricPrimitive>(sphereprim, material);
    } 

    throw omg::ParseException("unknown object type " + type);
}

template<typename NodeType>
std::vector<std::shared_ptr<NodeType>> YAMLParser::parse_list(const YAML::Node& node) {
    std::vector<std::shared_ptr<NodeType>> objs;
    for (auto & n : node) {
        objs.push_back(parse<NodeType>(n)); 
    }
    return objs;
}

template<>
std::vector<std::shared_ptr<Primitive>> YAMLParser::parse_list(const YAML::Node& node) {
    std::vector<std::shared_ptr<Primitive>> objs;

    for (auto & n : node) {
        objs.push_back(parse<Primitive>(n));
    }

    return objs;
}

template<>
std::shared_ptr<Material> YAMLParser::parse(const YAML::Node& node) {
    auto type = hard_require(node, "type").as<std::string>();
    if (type == "flat") {
        RGBColor color = hard_require(node, "color").as<Vec3>();
        return std::make_shared<FlatMaterial>(color);
    } else if (type == "blinn") {
        Vec3 ka = hard_require(node, "ka").as<Vec3>();
        Vec3 kd = hard_require(node, "kd").as<Vec3>();
        Vec3 ks = hard_require(node, "ks").as<Vec3>();
        RealValue glossiness = hard_require(node, "glossiness").as<RealValue>();
        return std::make_shared<BlinnMaterial>(ka, kd, ks, glossiness);
    }
    
    throw omg::ParseException("unknown material type " + type);
}

std::shared_ptr<Material> YAMLParser::get_material(const std::string& label) {
    auto it = _materials.find(label);
    if (it != _materials.end()) {
        return it->second;
    } else {
        throw omg::ParseException("missing material with label " + label);
    }
}


template<>
std::shared_ptr<RaytracerRunningSettings> YAMLParser::parse(const YAML::Node& node) {
    auto rt_settings = std::make_shared<RaytracerRunningSettings>();
    rt_settings->integrators = parse_list<Integrator>(hard_require(node, "integrators"));
    return rt_settings;
}

std::shared_ptr<RaytracerRunningSettings> YAMLParser::parse_running_settings() {
    return parse<RaytracerRunningSettings>(hard_require(root, "settings"));
}

template<>
std::shared_ptr<Background> YAMLParser::parse(const YAML::Node& node) {
    try {
        auto background_node = hard_require(node, "background");

        auto background_type = hard_require(background_node, "type").as<std::string>();

        if (background_type == "gradient") {
            auto colors = hard_require(background_node, "colors").as<std::vector<Vec3>>();
            if (colors.size() > 4)
                omg::logger.log(Logger::Type::WARNING, "parsing", "only first four gradient colors were considered");
            return std::make_shared<GradBilinearBackground>(colors);
        } else if (background_type == "solid") {
            auto color = hard_require(background_node, "color").as<Vec3>(); 
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
        auto target = hard_require(camera_node, "target").as<Vec3>();
        auto position = hard_require(camera_node, "position").as<Vec3>();
        auto up = hard_require(camera_node, "up").as<Vec3>();

        std::optional<Vec3> view_normal = std::nullopt;
        if (camera_node["view_normal"]) {
            view_normal = camera_node["view_normal"].as<Vec3>(); 
        }

        auto vpdims = parse<Camera::VpDims>(camera_node);

        std::shared_ptr<Camera> camera = nullptr;

        if (camera_type == "orthographic") {
            if (vpdims == nullptr)
                throw omg::ParseException("provide vpdims for ortho camera");
            camera = std::make_shared<OrthoCamera>(width, height, 
                    position, target, up, *vpdims);
        } else if (camera_type == "perspective") {
            float fdistance = hard_require(camera_node, "fdist").as<float>();
            if (vpdims != nullptr) {
                camera = std::make_shared<PerspectiveCamera>(width, height, position, target, up, *vpdims, fdistance);
            } else {
                float vaov = hard_require(camera_node, "v_angle").as<float>();
                if (camera_node["aspect_ratio"]) {
                    auto aspect_ratio = camera_node["aspect_ratio"].as<float>();
                    camera = std::make_shared<PerspectiveCamera>(width, height, position, target, up, aspect_ratio, fdistance, vaov);
                } else {
                    camera = std::make_shared<PerspectiveCamera>(width, height, position, target, up, fdistance, vaov);
                }
            }
        } else {
            throw omg::ParseException("undefined camera type " + camera_type); 
        }

        if (view_normal != std::nullopt) {
            camera->set_plane_normal(view_normal.value());
        }

        return camera;
    } catch (omg::ParseException & e) {
        throw;
    } catch (YAML::BadConversion & e) {
        throw omg::ParseException(e.what());
    }
    return nullptr;
}

template<>
std::shared_ptr<Integrator> YAMLParser::parse(const YAML::Node& integrator_node) {

    try {
        auto integrator_type = hard_require(integrator_node, "type").as<std::string>();

        auto spp = integrator_node["spp"] ? integrator_node["spp"].as<int>() : 1;
    
        std::shared_ptr<Integrator> integrator = nullptr;

        if (integrator_type == "flat") {
            integrator = std::make_shared<FlatIntegrator>(spp); 
        } else if (integrator_type == "depth") {
            auto near_color = defaulted_require(integrator_node, "near_color", RGBColor {255, 255, 255});
            auto far_color = defaulted_require(integrator_node, "far_color", RGBColor {0, 0, 0});
            if (integrator_node["near"] && integrator_node["far"]) {
                auto near = integrator_node["near"].as<float>();
                auto far = integrator_node["far"].as<float>();
                integrator = std::make_shared<DepthIntegrator>(near_color, far_color, near, far, spp); 
            } else {
                integrator = std::make_shared<DepthIntegrator>(near_color, far_color, spp); 
            }
        } else if (integrator_type == "normal_map") {
            integrator = std::make_shared<NormalMapIntegrator>(spp);
        } else if (integrator_type == "blinn") {
            integrator = std::make_shared<BlinnPhongIntegrator>(spp);
        } else {
            throw omg::ParseException("unknown integrator type " + integrator_type);
        }

        return integrator;

    } catch (omg::ParseException & e) {
        throw;
    } catch (YAML::BadConversion & e) {
        throw omg::ParseException(e.what());
    }
}

void YAMLParser::parse_materials(const YAML::Node& node) {
   for (auto& n : node) {
        auto label = hard_require(n, "label").as<std::string>();
        _materials[label] = parse<Material>(n);
   } 
}

template<>
std::shared_ptr<Light> YAMLParser::parse(const YAML::Node& light_node) {
    try {
        std::string type = hard_require(light_node, "type").as<std::string>();

        std::shared_ptr<Light> light = nullptr;

        auto intensity = hard_require(light_node, "intensity").as<Vec3>();

        if (type == "point") {
            auto position = hard_require(light_node, "position").as<Vec3>();
            light = std::make_shared<PointLight>(intensity, position);             
        } else if (type == "directional") {
            auto direction = hard_require(light_node, "direction").as<Vec3>();
            light = std::make_shared<DirectionalLight>(intensity, direction);             
        } else if (type == "ambient") {
            light = std::make_shared<AmbientLight>(intensity);             
        } else if (type == "spot") {
            auto position = hard_require(light_node, "position").as<Vec3>();
            auto point_at = hard_require(light_node, "point_at").as<Vec3>();
            auto falloff = hard_require(light_node, "falloff").as<float>();
            auto cutoff = hard_require(light_node, "cutoff").as<float>();
            light = std::make_shared<SpotLight>(intensity, position, point_at, falloff, cutoff);
        } else {
            throw omg::ParseException("unknown light type " + type);
        }
        return light;
    } catch (omg::ParseException & e) {
        throw;
    } catch (YAML::BadConversion & e) {
        throw omg::ParseException(e.what());
    }
}

template<>
std::shared_ptr<Scene> YAMLParser::parse(const YAML::Node& node) {
    // context
    auto camera = this->parse<Camera>(node);
    auto background = this->parse<Background>(node);
    std::vector<std::shared_ptr<Primitive>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    // scene
    if (node["scene"]) {
        auto scene_node = node["scene"];

        if (scene_node["materials"]) {
            parse_materials(scene_node["materials"]);
        }

        if (scene_node["lights"]) {
            lights = this->parse_list<Light>(scene_node["lights"]);
        }

        if (scene_node["objects"]) {
            objects = this->parse_list<Primitive>(scene_node["objects"]);
        }
    }
    auto scene = std::make_shared<Scene>(background, camera, objects, lights);
    return scene;
}

void YAMLParser::init_from_text(const std::string & text) {
    YAML::Node root = YAML::Load(text);
    this->root = hard_require(root, "raytracer");
}

void YAMLParser::init_from_file(const std::string & file_path) {
    YAML::Node root = YAML::LoadFile(file_path);
    this->root = hard_require(root, "raytracer");
}

std::shared_ptr<Scene> YAMLParser::parse_scene() {
    return this->parse<Scene>(root);
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

