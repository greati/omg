#ifndef _OMG_TRANSFORM_
#define _OMG_TRANSFORM_

#include "omg/common.h"

namespace omg {

class Transform {
    
    private:

        Matrix4x4 mat, mInv;

    public:

        Transform() {}

        Transform(const Matrix4x4& mat)
        : mat {mat}, mInv {tao::inverse(mat)} {}

        Transform(const Matrix4x4& mat, const Matrix4x4& mInv)
            : mat {mat}, mInv{mInv} {/* empty */}

        /**
         * Produces the inverse transform.
         *
         * @param t a transform
         * */
        friend Transform inverse(const Transform& t) {
            return Transform(t.mInv, t.mat);
        }

        /**
         * Produces the transpose transform.
         *
         * @param t a transform
         * */
        friend Transform transpose(const Transform& t) {
            return Transform(t.mat.t(), t.mInv.t());
        }

        /**
         * Test for equality with precision.
         * */
        bool eq(const Transform& t2, double precision=0.001) {
            return mat.eq(t2.mat, precision);
        }

        /*
         * Test for equality.
         * */
        bool operator==(const Transform& t1) {
            return mat == t1.mat;
        }

        /**
         * Test for equality with precision.
         * */
        bool neq(const Transform& t2, double precision=0.001) {
            return not mat.eq(t2.mat, precision);
        }

        /*
         * Test for equality.
         * */
        bool operator!=(const Transform& t1) {
            return not (mat == t1.mat);
        }

        /**
         * Check if this is an identity matrix.
         * */
        bool is_identity() const {
            return tao::is_identity(mat); 
        } 
};



};

#endif
