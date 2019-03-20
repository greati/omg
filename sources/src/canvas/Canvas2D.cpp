#include "canvas/Canvas2D.h"
#include <iostream>

template<int N>
PixelValue<N> Canvas2D<N>::at(Point2 point) const {
    auto [start, end] = real_pos(point);
    PixelValue<N> v;
    for (auto i = start, j = 0; i < end; ++i, ++j) {
        v[j] = _data[i];
    }
    return v;
}

template<int N>
void Canvas2D<N>::set(Point2 point, PixelValue<N> value) const {
    auto [start, end] = real_pos(point);
    for (int i = start, j = 0; i < end; ++i, ++j) {
        _data[i] = value[j];
    }
}

template<int N>
void Canvas2D<N>::set(Point2 point, PixelValue<N> value, std::nothrow_t) const noexcept {
    try {
        this->set(point, value);
    } catch (std::exception& e) {/* empty */}
}

template<int N>
std::pair<int, int> Canvas2D<N>::real_pos(const Point2 & point) const {
    validate(point);
    auto [i, j] = point;
    auto start = i*_width*N + j*N;
    return {start, start + N};
}

template<int N>
Canvas2D<N>::Canvas2D(int width, int height) : _width{width}, _height{height} {
    this->_data = std::make_unique<unsigned char[]>(width * height * N);
}

template<int N>
void Canvas2D<N>::validate(const Point2 & point) const {
    auto [x, y] = point;

    if (y < 0 || y >= _width)
        throw std::invalid_argument("point outsite the width range, " + std::to_string(x));
    if (x < 0 || x >= _height)
        throw std::invalid_argument("point outsite the height range, " + std::to_string(y));
}

template<int N>
void Canvas2D<N>::clear(const PixelValue<N> & value) {
    for (int i = 0; i < _height; ++i)
        for (int j = 0; j < _width; ++j)
            this->set({i, j}, value);
}

template<int N>
void Canvas2D<N>::reset(Size<2> size) {
    auto [_height, _width] = size;
    this->_data.reset(new unsigned char[int(_width) * int(_height) * N]);
    this->_width = _width;
    this->_height = _height;
}

template class Canvas2D<3>;

