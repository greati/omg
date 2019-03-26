#include "printer/NetpbmPrinter.h"
#include <iomanip>

template<typename T, typename ConfigKeyType>
std::string NetpbmPrinter<T, ConfigKeyType>::convert(
        const T* data, 
        const Configs<ConfigKeyType> & configs) const {

    validate(configs);

    NetpbmType type = (NetpbmType) configs.template get<int>(NetpbmParams::PBM_TYPE);
    NetpbmEncoding encoding = (NetpbmEncoding) configs.template get<int>(NetpbmParams::PBM_ENCODING);
    auto width = configs.template get<int>(NetpbmParams::IMAGE_WIDTH);
    auto height = configs.template get<int>(NetpbmParams::IMAGE_HEIGHT);
    auto channels = configs.template get<int>(NetpbmParams::IMAGE_CHANNELS);
    auto max_intensity = configs.template get<int>(NetpbmParams::MAX_INTENSITY);

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

template<typename T, typename ConfigKeyType>
void NetpbmPrinter<T, ConfigKeyType>::validate(const Configs<ConfigKeyType> & configs) const {
    
}

template class NetpbmPrinter<unsigned char, NetpbmParams>;
