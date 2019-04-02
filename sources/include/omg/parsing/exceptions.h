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

        std::string _info;

    public:

        ParseException(const std::string& info) : _info {info} {/* empty */}

        std::string get_info() const { return _info; }
};
};
#endif
