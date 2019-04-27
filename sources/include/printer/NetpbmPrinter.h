#ifndef __NETPBMPRINTER__
#define __NETPBMPRINTER__

#include "Printer.h"

namespace netpbm {

enum NetpbmType {
    BIT_MAP = 0,
    GRAY_MAP = 1,
    PIX_MAP = 2
};

enum NetpbmEncoding {
    ASCII = 0,
    BINARY = 1
};

struct Options {
   inline static const std::string IMAGE_WIDTH = "width";
   inline static const std::string IMAGE_HEIGHT = "height";
   inline static const std::string IMAGE_CHANNELS = "channels";
   inline static const std::string PBM_TYPE = "pbm_type";
   inline static const std::string PBM_ENCODING = "pbm_encoding";
   inline static const std::string MAX_INTENSITY = "max_intensity";
};

/**
 * A printer for the netpbm format.
 * 
 * @author Vitor Greati
 * */
template<typename T>
class NetpbmPrinter : public Printer<T> {

    public:

        NetpbmPrinter(const Configs<std::string>& configs) : Printer<T>(configs) {
            this->_ext = ".ppm";
        }

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
        std::string convert(const T* data) const override;

        void validate(const Configs<>& configs) const override;

};
};
#endif
