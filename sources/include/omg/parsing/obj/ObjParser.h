#ifndef _OBJ_PARSER_
#define _OBJ_PARSER_

#include <vector>
#include <string>
#include <memory>

namespace omg {
class Triangle;
class ObjParser {

    public:

        virtual 
        std::vector<std::shared_ptr<Triangle>> 
        parse_tri_mesh(const std::string& file_name, bool bfc) const = 0;   

};
};

#endif
