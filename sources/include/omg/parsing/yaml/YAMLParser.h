#ifndef __YAMLPARSER__
#define __YAMLPARSER__

#include "omg/parsing/Parser.h"
#include "yaml-cpp/yaml.h"
#include "omg/materials/Material.h"
#include "omg/raytracer/TransformCache.h"
#include <stack>

namespace omg {
/**
 * A scene parser using YAML.
 *
 * @author Vitor Greati
 * */
class YAMLParser : public Parser {

    public:

        void init_from_text(const std::string & text) override;

        void init_from_file(const std::string & file_path) override; 

        std::shared_ptr<Scene> parse_scene() override;

        std::shared_ptr<RaytracerRunningSettings> parse_running_settings() override;

    private:

        std::map<std::string, std::shared_ptr<Material>> _materials;

        YAML::Node root;

        std::stack<Transform> transStack;

        Transform curTrans;

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
         * Parse and store objects.
         * */
        void parse_materials(const YAML::Node& node);

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

        /**
         * Require a parameter, providing a default value when
         * not available.
         *
         * @param curr_node
         * @param node_name
         * @param def
         * @return the default value
         * */
        template<typename DefType>
        DefType defaulted_require(const YAML::Node & curr_node, 
                const std::string& node_name, 
                const DefType& def) const;

        /**
         * Searches a parsed material stored in the materials dictionary.
         *
         * @param label the name of the material
         * @param node in case lookup fail, try to parse a material using this node
         * @param a pointer to a material
         * */
        std::shared_ptr<Material> get_material(const std::string& label);

};
};
#endif
