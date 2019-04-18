#ifndef __PRINTER__
#define __PRINTER__

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <map>
#include "configs/Configs.h"

/**
 * Interface for printers.
 *
 * @author Vitor Greati
 * */
template<typename T>
class Printer {

    protected:

        Configs<std::string> _configs;

    public:


        Printer() {}

        Printer(const decltype(_configs) & configs) : _configs {configs} { /** empty */}

        /**
         * Print the formatted data into the std output.
         *
         * @param data the data
         * */
        inline void print(const T* data) { 
            std::cout << convert(data, _configs); 
        }

        /**
         * Print the formatted data into a file.
         *
         * @param data the data
         * @param dest file pathd destination
         * */
        inline void print(
                const T* data,
                const std::string & dest) {
            std::ofstream os;
            os.open(dest);
            if (os.is_open()) {
                os << convert(data);
                os.close();
            } else {
                throw std::logic_error("failed to open the file " + dest);
            }
        }

    protected:

        /**
         * Convert the data array into a string representation.
         *
         * @param data data array
         * @param size image size
         * @param config configuration options
         * @return string representation of that data
         * */
        virtual std::string convert(
                const T* data) const = 0;

        virtual void validate(const Configs<std::string>& configs) const {}

};

#endif
