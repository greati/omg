#ifndef __PARSER_EXCEPTIONS__
#define __PARSER_EXCEPTIONS__

#include <exception>

namespace omg {
/**
 * Parsing exception.
 *
 * @author Vitor Greati
 * */
class ParseException : public std::exception {

    private:

        const char* _info;

    public:

        ParseException(const std::string& info) : _info {info.c_str()} {/* empty */}

        const char* get_info() const { return _info; }
};
};
#endif
