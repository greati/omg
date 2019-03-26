#ifndef __NETPBMPRINTER__
#define __NETPBMPRINTER__

#include "Printer.h"

enum class NetpbmParams {
    IMAGE_WIDTH = 0,
    IMAGE_HEIGHT = 1,
    IMAGE_CHANNELS = 2,
    PBM_TYPE = 3,
    PBM_ENCODING = 4,
    MAX_INTENSITY = 5
};

enum NetpbmType {
    BIT_MAP = 0,
    GRAY_MAP = 1,
    PIX_MAP = 2
};

enum NetpbmEncoding {
    ASCII = 0,
    BINARY = 1
};

/**
 * A printer for the netpbm format.
 * 
 * @author Vitor Greati
 * */
template<typename T, typename ConfigKeyType = NetpbmParams>
class NetpbmPrinter : public Printer<T, ConfigKeyType> {

    public:

        std::map<std::pair<NetpbmType, NetpbmEncoding>, std::string> magic_number_table = {
            {{BIT_MAP, ASCII}, "P1"},
            {{BIT_MAP, BINARY}, "P4"},
            {{GRAY_MAP, ASCII}, "P2"},
            {{GRAY_MAP, BINARY}, "P5"},
            {{PIX_MAP, ASCII}, "P3"},
            {{PIX_MAP, BINARY}, "P6"},
        };

    protected:

        /**
         * Convert the data array into a string in netpbm format
         *
         * @param data data array
         * @return string representation of that data
         * */
        std::string convert(const T* data, const Configs<ConfigKeyType> & configs) const override;

    private:

        /**
         * Validate configuration options.
         *
         * @param config the configuration
         * */
        void validate(const Configs<ConfigKeyType> & params) const;

};

template class Configs<NetpbmParams>;

#endif
