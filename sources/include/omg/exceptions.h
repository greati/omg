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
        int _line {-1};
        int _col {-1};

    public:

        ParseException(const std::string& info, int line, int col) 
            : _info {info}, _line {line}, _col {col} {/* empty */}

        ParseException(const std::string& info) : _info {info} {/* empty */}

        inline int get_line() const { return _line; }

        inline int get_col() const { return _col; }

        std::string get_info() const { 
            return _info;
        }

        std::string message() const noexcept {
            std::string wh = "";
            if (_col != -1 && _line != -1)
                wh += "(" + std::to_string(_line) + "," + std::to_string(_col) + ") " + _info;
            else
                wh = _info;
            return wh;
        }
};
};
#endif
