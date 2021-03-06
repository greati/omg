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
#include "omg/objects/Triangle.h"
#include "omg/objects/TriangleMesh.h"
#include "omg/objects/ListAggregate.h"
#include "omg/parsing/obj/CyObjParser.h"
#include "omg/accelerators/BVHAccel.h"
#include "omg/objects/Transform.h"
#include "omg/objects/TransformedPrimitive.h"
#include "omg/raytracer/RaytracerEngine.h"
#include <iostream>

using namespace omg;

template<>
struct YAML::convert<Vec2> {
    static YAML::Node encode(const Vec2& rhs) {
        YAML::Node node;
        return node;
    }
    static bool decode(const YAML::Node& node, Vec2 & rgbColor) {
        if (!node.IsSequence() || node.size() != 2)
            return false;

        rgbColor(0) = node[0].as<float>(); 
        rgbColor(1) = node[1].as<float>(); 
        return true;
    }
};

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
struct YAML::convert<BVHAccel::SplitMethod> {
    static YAML::Node encode(const BVHAccel::SplitMethod& rhs) {
        YAML::Node node;
        return node;
    }
    static bool decode(const YAML::Node& node, BVHAccel::SplitMethod & splitMethod) {

        auto method = node.as<std::string>();
        if (method == "middle")
             splitMethod = BVHAccel::SplitMethod::Middle;
        else if (method == "equal_counts")
             splitMethod = BVHAccel::SplitMethod::EqualCounts;
        else return false;

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
    // Get transforms
    std::shared_ptr<Transform> otw, wto;
    RaytracerEngine::transform_cache.lookup(curTrans, otw, wto);
    return std::make_shared<Sphere>(radius, center, otw, wto);
}


template<>
std::vector<std::shared_ptr<Triangle>> YAMLParser::parse_list(const YAML::Node& node) {
    auto n_triangles = hard_require(node, "ntriangles").as<int>();
    auto indices = hard_require(node, "indices").as<std::vector<int>>(); 
    auto vertices = hard_require(node, "vertices").as<std::vector<Vec3>>();
    auto normals = hard_require(node, "normals").as<std::vector<Vec3>>();
    std::vector<Point2> uvs; //TODO ask for uvs
    bool bfc = node["bfc"] ? node["bfc"].as<bool>() : true;
    bool clockwise = node["clockwise"] ? node["clockwise"].as<bool>() : false;

    std::shared_ptr<Transform> otw, wto;
    RaytracerEngine::transform_cache.lookup(curTrans, otw, wto);

    return Triangle::create_triangle_mesh(otw, wto, bfc, n_triangles, indices.data(),
            vertices.size(), vertices.data(), normals.data(), uvs.data(), false, clockwise);
}

template<>
std::shared_ptr<Primitive> YAMLParser::parse(const YAML::Node& node);

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
std::shared_ptr<Primitive> YAMLParser::parse(const YAML::Node& node) {

    std::shared_ptr<Primitive> primitive = nullptr;

    auto type = hard_require(node, "type").as<std::string>();

    bool is_transformed = false;

    if (node["transform"]) {

        is_transformed = true;

        auto node_transform = node["transform"];
        auto transform_list = parse_list<Transform>(node["transform"]);

        auto comp_result = Transform::compose(transform_list);

        // Take the transform
        std::shared_ptr<Transform> t;
        RaytracerEngine::transform_cache.lookup(*comp_result, t);
        // Put it in the stack
        transStack.push(*t);
        // Update current
        curTrans = curTrans * (*t);
    }

    if (type == "aggregate") {
        
        std::vector<std::shared_ptr<Primitive>> objects; 

        if (node["objects"])
            objects = this->parse_list<Primitive>(node["objects"]);

        if (node["structure"]) {
            auto node_structure = node["structure"];
            auto structure_type = hard_require(node_structure, "type").as<std::string>();

            if (structure_type == "bvh") {
                auto max_prims_nodes = node_structure["max_prims_node"].as<int>();
                auto split_method = node_structure["split_method"].as<BVHAccel::SplitMethod>();
                primitive = std::make_shared<BVHAccel>(objects, max_prims_nodes, split_method);
            } else if (structure_type == "list") {
                primitive = std::make_shared<ListAggregate>(objects);
            } else throw omg::ParseException("unknown structure type " + type);
        } else 
            primitive = std::make_shared<ListAggregate>(objects);

    } else if (type == "sphere") {
        auto sphereprim = parse<Sphere>(node);
        auto material = get_material(hard_require(node, "material").as<std::string>());
        primitive = std::make_shared<GeometricPrimitive>(sphereprim, material);
    } else if (type=="triangle_mesh") {
        auto material = get_material(hard_require(node, "material").as<std::string>());

        std::vector<std::shared_ptr<Triangle>> triangles;

        //> When a obj file is specified
        if (node["obj_file"]) {
            CyObjParser cyobjparser;
            try {
                auto obj_file = node["obj_file"].as<std::string>();
                bool bfc = node["bfc"] ? node["bfc"].as<bool>() : true;
                bool compute_normals = node["compute_normals"] ? node["compute_normals"].as<bool>() : false;
                bool clockwise = node["clockwise"] ? node["clockwise"].as<bool>() : false;

                std::shared_ptr<Transform> otw, wto;
                RaytracerEngine::transform_cache.lookup(curTrans, otw, wto);

                triangles = cyobjparser.parse_tri_mesh(otw, wto, obj_file, bfc, compute_normals, clockwise); 
            } catch (omg::ParseException& e) {
                throw;
            }
        //> Parse manually created mesh
        } else {
            triangles = parse_list<Triangle>(node); 
        }
        //> Create primitives
        std::vector<std::shared_ptr<Primitive>> t_primitives;
        for (auto& t : triangles)
            t_primitives.push_back(std::make_shared<GeometricPrimitive>(t, material));
        primitive = std::make_shared<BVHAccel>(t_primitives, 4, BVHAccel::SplitMethod::Middle);
    } else 
        throw omg::ParseException("unknown object type " + type);

    if (is_transformed) {
        // update cur transform
        auto t = transStack.top();
        //curTrans = inverse(t) * curTrans;
        curTrans = curTrans * inverse(t);
        // pop stack
        transStack.pop();
    }

    return primitive;
}

template<>
std::shared_ptr<Material> YAMLParser::parse(const YAML::Node& node) {
    auto type = hard_require(node, "type").as<std::string>();
    if (type == "flat") {
        RGBColor color = hard_require(node, "color").as<Vec3>();
        return std::make_shared<FlatMaterial>(color);
    } else if (type == "blinn") {
        Vec3 ka = hard_require(node, "ambient").as<Vec3>();
        Vec3 kd = hard_require(node, "diffuse").as<Vec3>();
        Vec3 ks = hard_require(node, "specular").as<Vec3>();
        Vec3 km = defaulted_require(node, "mirror", Vec3 {0.0, 0.0, 0.0});
        RealValue glossiness = hard_require(node, "glossiness").as<RealValue>();
        return std::make_shared<BlinnMaterial>(ka, kd, ks, km, glossiness);
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
            cutoff = (cutoff * M_PI) / 180;
            falloff = (falloff * M_PI) / 180;
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
std::shared_ptr<Transform> YAMLParser::parse(const YAML::Node& node) {
    std::shared_ptr<Transform> transform;
    
    auto type = hard_require(node, "type").as<std::string>();

    if (type=="rotation_x") {
        auto angle = hard_require(node, "angle").as<float>();
        RaytracerEngine::transform_cache.lookup(Transform::make_rotateX(angle), transform);
    } else if (type=="rotation_y") {
        auto angle = hard_require(node, "angle").as<float>();
        RaytracerEngine::transform_cache.lookup(Transform::make_rotateY(angle), transform);
    } else if (type=="rotation_z") {
        auto angle = hard_require(node, "angle").as<float>();
        RaytracerEngine::transform_cache.lookup(Transform::make_rotateZ(angle), transform);
    } else if (type=="rotation") {
        auto angle = hard_require(node, "angle").as<float>();
        auto axis = hard_require(node, "axis").as<Vec3>();
        RaytracerEngine::transform_cache.lookup(Transform::make_rotate(angle, axis), transform);
    } else if (type=="scale") {
        auto xyz = hard_require(node, "xyz").as<Vec3>();
        RaytracerEngine::transform_cache.lookup(Transform::make_scale(xyz(0), xyz(1), xyz(2)), transform);
    } else if (type=="translation") {
        auto delta = hard_require(node, "delta").as<Vec3>();
        RaytracerEngine::transform_cache.lookup(Transform::make_translate(delta), transform);
    }

    return transform;
}

template<>
std::shared_ptr<Scene> YAMLParser::parse(const YAML::Node& node) {
    // initialize transform cache
    std::shared_ptr<Transform> tIdentity;
    RaytracerEngine::transform_cache.lookup(Matrix4x4::identity(), tIdentity);
    curTrans = *tIdentity;
    transStack.push(curTrans);

    // context
    auto camera = this->parse<Camera>(node);
    auto background = this->parse<Background>(node);
    std::vector<std::shared_ptr<Primitive>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<AggregatePrimitive> aggregate;
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
            aggregate = std::make_shared<ListAggregate>(objects);
        }
    }
    auto scene = std::make_shared<Scene>(background, camera, aggregate, lights);
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

