#ifndef __STD_LOGGER__
#define __STD_LOGGER__


#include "Logger.h"
#include "ProgressBar.hpp"
#include <iostream>
#include <iomanip>
#include <map>

/**
 * An interface for loggers, using the 
 * standard output.
 *
 * @author Vitor Greati
 * */
class StandardLogger : Logger {
    
    private:

        std::map<std::string, ProgressBar> progress_bars;
        std::shared_ptr<ProgressBar> progress_bar;

    public:

        /**
         * Basic constructor.
         * */
        StandardLogger(const std::string& prefix) : Logger {prefix} {/*empty*/}
        
        void print_header(const Logger::Type & type, const std::string & label, const std::string & msg) const {
            std::cout << this->_prefix;
            std::cout << std::setw(15) << std::right;
            std::cout << "(" + label + ")";
        }

        /**
         * Print a standard string log.
         *
         * @param type the type of the log
         * @param msg the message
         * */
        void log(const Logger::Type & type, const std::string & label, const std::string & msg) const {
            print_header(type, label, msg);
            std::string res = "";
            res += " ";
            res += label_msg_type(type);
            res += " ";
            std::cout << std::setw(20) << std::right << res;
            std::cout << msg;
            std::cout << std::endl;
        }

        /**
         * Print a log with file location info.
         *
         * @param type the type of the log
         * @param msg the log message
         * @param line the line to where the log refers
         * @param col the col to where the log refers
         * */
        void log(const Logger::Type & type, const std::string& label, const std::string & msg, int line, int col) const {
            std::string res = this->_prefix;
            res += " ";
            res += "(";
            res += label;
            res += ")";
            res += " ";
            res += label_msg_type(type);
            res += " ";
            res += "[ " + std::to_string(line) + "," + std::to_string(col) + " ]";
            res += " ";
            res += msg;
            std::cout << res;
            std::cout << std::endl;
        }

        /**
         * Gives a label for a type of message.
         *
         * @param type the type
         * @param the label representing the type
         * */
        std::string label_msg_type(const Logger::Type& type) const {
            switch(type) {
                case Logger::Type::INFO:
                    return "\033[1;36mINFO\033[0m";
                case Logger::Type::WARNING:
                    return "\033[1;33mWARNING\033[0m";
                case Logger::Type::ERROR:
                    return "\033[1;31mERROR\033[0m";
                case Logger::Type::SUCCESS:
                    return "\033[1;32mSUCCESS\033[0m";
            }
            return "\033[1;35mUNKNOWN\033[0m";
        }

        void make_progress_bar(const std::string& name, int limit = 0, int width = 0) override {
            progress_bar = std::make_shared<ProgressBar>(limit, width);
        }

        void update_progress_bar(const std::string& name, int delta=1) override {
            ++(*progress_bar);
            progress_bar->display();
        }

        virtual void finish_progress_bar(const std::string& name) {
            progress_bar->done();
        }
};

#endif
