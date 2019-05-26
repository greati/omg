#ifndef __LOGGER__
#define __LOGGER__

#include <string>

/**
 * An interface for loggers.
 *
 * @author Vitor Greati
 * */
class Logger {
    
    protected:

        std::string _prefix;      /** the prefix for the logger */

    public:

        /**
         * Basic constructor.
         * */
        Logger(const std::string& prefix) : _prefix {prefix} {/*empty*/}

        /**
         * Types of logs.
         * */
        enum class Type {
            INFO = 0,
            WARNING = 1,
            ERROR = 2,
            SUCCESS = 3
        };
        
        /**
         * Print a standard string log.
         *
         * @param type the type of the log
         * @param msg the message
         * */
        virtual void log(const Logger::Type & type, const std::string& label, const std::string & msg) const = 0;

        /**
         * Print a log with file location info.
         *
         * @param type the type of the log
         * @param msg the log message
         * @param line the line to where the log refers
         * @param col the col to where the log refers
         * */
        virtual void log(const Logger::Type & type, const std::string& label, const std::string & msg, int line, int col) const = 0;

        /**
         * Gives a label for a type of message.
         *
         * @param type the type
         * @param the label representing the type
         * */
        virtual std::string label_msg_type(const Logger::Type& type) const = 0;

        /**
         * Build a new named progress bar.
         * */
        virtual void make_progress_bar(const std::string& name, int limit = 0, int current = 0) = 0;

        /**
         * Update an existing progress bar.
         * */
        virtual void update_progress_bar(const std::string& name, int delta=1) = 0;

        /**
         * Finish an existing progress bar.
         * */
        virtual void finish_progress_bar(const std::string& name) = 0;

};

#endif
