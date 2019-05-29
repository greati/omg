#ifndef _CY_OBJ_PARSER_
#define _CY_OBJ_PARSER_

#include "ObjParser.h"

namespace omg {
class Triangle;
class CyObjParser : public ObjParser {

    public:

        std::vector<std::shared_ptr<Triangle>> 
        parse_tri_mesh(const std::string& file_name, bool bfc) const override;   

};
};

#endif
