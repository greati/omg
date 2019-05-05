#include "printer/NetpbmPrinter.h"
#include <iomanip>

using namespace netpbm;

template<typename T>
std::string NetpbmPrinter<T>::convert(
        const T* data) const {

    NetpbmType type = (NetpbmType) this->_configs.template get<int>(Options::PBM_TYPE);
    NetpbmEncoding encoding = (NetpbmEncoding) this->_configs.template get<int>(Options::PBM_ENCODING);
    auto width = this->_configs.template get<int>(Options::IMAGE_WIDTH);
    auto height = this->_configs.template get<int>(Options::IMAGE_HEIGHT);
    auto channels = this->_configs.template get<int>(Options::IMAGE_CHANNELS);
    auto max_intensity = this->_configs.template get<int>(Options::MAX_INTENSITY);

    auto magic_number = magic_number_table.at({type, encoding});

    std::stringstream ss;
    ss << magic_number << std::endl;
    ss << width << " " << height << std::endl;
    ss << max_intensity << std::endl;
    
    for (auto i = 0; i < width * height * channels; ++i) {
        ss << (int) data[i] << " ";
        if ( (i+1) % (width*channels) == 0 )
            ss << std::endl;
    }
    
    return ss.str();
}

template<typename T>
void NetpbmPrinter<T>::validate(const Configs<>& configs) const {
    
}

template class NetpbmPrinter<unsigned char>;
