#include "buffer/Buffer.h"
#include <iostream>

using namespace rayt;

template<int N, typename StoredType>
Buffer<N,StoredType>::Buffer(int width, int height) : _width{width}, _height{height} {
    this->_data = std::make_unique<StoredType[]>(width * height * N);
}

template<int N, typename StoredType>
StoredType& Buffer<N,StoredType>::operator()(int row, int col, int channel) {
    if (channel >= N || channel < 0)
        throw std::invalid_argument("invalid channel index, should be between 0 and " + std::to_string(N));
    auto [start, end] = real_pos(row, col);
    return this->_data[start + channel];
}

template<int N, typename StoredType>
StoredType& Buffer<N,StoredType>::operator()(const Buffer<N,StoredType>::Cartesian& cartesian, int channel) {
    auto [row, col] = this->cart_to_rowcol(cartesian);
    return (*this)(row, col, channel);
}

template<int N, typename StoredType>
void Buffer<N,StoredType>::set(int row, int col, const Buffer<N,StoredType>::Value& value){
    auto [start, end] = real_pos(row, col);
    for (int i = start, j = 0; i < end; ++i, ++j) {
        _data[i] = value[j];
    }
}

template<int N, typename StoredType>
void Buffer<N,StoredType>::set(const Buffer<N,StoredType>::Cartesian& cartesian, const Buffer<N,StoredType>::Value& value) {
    auto [row, col] = this->cart_to_rowcol(cartesian);
    this->set(row, col, value);
}

template<int N, typename StoredType>
typename Buffer<N,StoredType>::Value Buffer<N,StoredType>::operator()(int row, int col) const {
    auto [start, end] = real_pos(row, col);
    Buffer<N,StoredType>::Value v;
    for (auto i = start, j = 0; i < end; ++i, ++j) {
        v[j] = _data[i];
    }
    return v;
}

template<int N, typename StoredType>
typename Buffer<N,StoredType>::Value Buffer<N,StoredType>::operator()(const Buffer<N,StoredType>::Cartesian& cartesian) const {
    auto [row, col] = this->cart_to_rowcol(cartesian);
    return (*this)(row, col);
}

template<int N, typename StoredType>
std::pair<int, int> Buffer<N,StoredType>::real_pos(int row, int col) const {
    validate(row, col);
    auto start = row*_width*N + col*N;
    return {start, start + N};
}

template<int N, typename StoredType>
void Buffer<N,StoredType>::validate(int row, int col) const {
    if (col < 0 || col >= _width)
        throw std::invalid_argument("point outside the width range, " + std::to_string(col));
    if (row < 0 || row >= _height)
        throw std::invalid_argument("point outside the height range, " + std::to_string(row));
}

template<int N, typename StoredType>
void Buffer<N,StoredType>::clear(const Buffer<N,StoredType>::Value & value) {
    for (int i = 0; i < _height; ++i)
        for (int j = 0; j < _width; ++j)
            this->set({i, j}, value);
}

template<int N, typename StoredType>
void Buffer<N,StoredType>::reset(int width, int height) {
    this->_data.reset(new StoredType[int(width) * int(height) * N]);
    this->_width = width;
    this->_height = height;
}

template<int N, typename StoredType>
std::pair<int, int> Buffer<N,StoredType>::cart_to_rowcol(const Buffer<N, StoredType>::Cartesian& cartesian) const {
    auto [col, row] = cartesian;
    return {_height - 1 - row, col};
}

template class Buffer<3>;
