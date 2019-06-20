#ifndef _OBJ_PARSER_
#define _OBJ_PARSER_

#include <vector>
#include <string>
#include <memory>

namespace omg {
class Triangle;
class Transform;
class ObjParser {

    public:

        virtual 
        std::vector<std::shared_ptr<Triangle>> 
        parse_tri_mesh(
            const std::shared_ptr<Transform>& object_to_world,
            const std::shared_ptr<Transform>& world_to_object,
            const std::string& file_name, bool bfc, bool compute_normals, bool clockwise) const = 0;   

};
};

#endif
