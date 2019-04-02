#ifndef __BUFFER__
#define __BUFFER__

#include <memory>
#include "omg/common.h"

namespace omg {
/**
 * A buffer for 2D images.
 *
 * @author Vitor Greati
 */
template<int N, typename StoredType = unsigned char>
class Buffer {

    private:

        using Value = std::array<StoredType, N>;    /** A value made by components */
        using Cartesian = std::pair<int, int>;      /** Column and line */

        int _width;                                 /** Buffer width */
        int _height;                                /** Buffer height */
        std::unique_ptr<StoredType[]> _data;        /** Buffer values components */

    public:
        
        /**
         * Constructor based on scene size.
         *
         * @param width the scene width
         * @param height the scene height
         * */
        Buffer(int width, int height);

        /**
         * Access a value in the buffer, origin is
         * upper left corner (row, col).
         *
         * @param row the row
         * @param col the col
         * @return the value at the (row, col) position
         * */
        Value operator()(int row, int col) const;

        /**
         * Access a value in the buffer, origin
         * is lower left corder (col, row).
         *
         * @param cartesian the cartesian coordinates (col, row)
         * @return the value at the (col, row) position
         * */
        Value operator()(const Cartesian& cartesian) const;

        /**
         * Modify a value at the (row, col) position,
         * origin is upper left corner.
         *
         * @param row the row
         * @param col the col
         * @param channel the channel
         * @return the value at (row, col) position
         * */
        StoredType& operator()(int row, int col, int channel);

        /**
         * Modify a value at the (col, row) position,
         * origin is lower left corner.
         *
         * @param cartesian the cartesian coordinates (col, row)
         * @param channel the channel
         * @return a reference to the value at (col, row)
         * */
        StoredType& operator()(const Cartesian& cartesian, int channel);

        /**
         * Set directly a composed value in the buffer,
         * (row, col) based.
         *
         * @param row the row
         * @param col the col
         * @param v the value
         * */
        void set(int row, int col, const Value& v);

        /**
         * Set directly a composed value in the buffer,
         * (col, row) based.
         *
         * @param cartesian cartesian coordinates
         * @param v the value
         * */
        void set(const Cartesian& cartesian, const Value& v);

        /**
         * The number of channels in the canvas.
         *
         * @return the number of channels
         * */
        inline int channels() const noexcept { return N; }

        /**
         * The width (number of cols).
         *
         * @return the buffer width
         * */
        inline int width() const noexcept { return _width; }

        /**
         * The height (number of rows)
         *
         * @return the buffer height
         * */
        inline int height() const noexcept { return _height; }

        /**
         * Recreate the buffer data
         *
         * @param width the new width
         * @param height the new height
         * */
        void reset(int width, int height);

        /**
         * Set a value in every position
         *
         * @param value value to be put in every position
         * */
        void clear(const Value & value);

        /**
         * A pointer to the canvas data.
         *
         * @return a pointer to the canvas data
         * */
        inline unsigned char* data() noexcept { return _data.get(); }

    private:
    
        /**
         * Validates a given point with respect to the width and height range.
         *
         * @param point the point
         * */
        void validate(int row, int col) const;
        
        /**
         * Given a point, return its real position inside the data member.
         *
         * @param point the point
         * @return the real position in the data member, as a range
         * */
        std::pair<int, int> real_pos(int row, int col) const;

        /**
         * Transform cartesian (col, row) llc to (row, col) ulc.
         *
         * */
        std::pair<int, int> cart_to_rowcol(const Cartesian& cartesian) const;
       
};
};
#endif

