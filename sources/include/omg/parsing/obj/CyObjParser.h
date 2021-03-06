#ifndef _CY_OBJ_PARSER_
#define _CY_OBJ_PARSER_

#include "ObjParser.h"

namespace omg {
class Triangle;
class CyObjParser : public ObjParser {

    public:

        std::vector<std::shared_ptr<Triangle>> 
        parse_tri_mesh(
            const std::shared_ptr<Transform>& object_to_world,
            const std::shared_ptr<Transform>& world_to_object,
            const std::string& file_name, bool bfc, bool compute_normals, bool clockwise) const override;   

};
};

#endif
