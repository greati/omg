#ifndef __CONFIGS__
#define __CONFIGS__

#include <string>
#include <map>
#include <variant>

/**
 * Represents a bundle of configurations.
 *
 * @author Vitor Greati
 * */
template<typename KeyType = std::string>
class Configs {

    protected:

        using ConfigValue = std::variant<char, int, long, float, double, std::string>;
        using ConfigsData = std::map<KeyType, ConfigValue>;

    private:

        ConfigsData _configs;   /**< Configuration data */

    public:

        /**
         * Construtor based on data.
         *
         * @param configs configs data
         * */
        Configs(ConfigsData configs) : _configs {configs} {/*empty*/}

        /**
         * Takes a value from the configurations and try to
         * convert it to a given type.
         *
         * @param key the key
         * @return the value of the indicated type
         * */
        template<typename GetType = std::string>
        inline GetType get(const KeyType & key) const {
            return std::get<GetType>(_configs.at(key));
        }

};

#endif
