#ifndef _OMG_TRANSFORM_
#define _OMG_TRANSFORM_

#include "omg/common.h"
#include "omg/objects/Bounds3.h"
#include "tao/geometry/geometry.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include "omg/raytracer/Ray.h"

namespace omg {

/**
 * Represents a geometric transform.
 *
 * @author Vitor Greati
 * */
class Transform {
    
    private:

        Matrix4x4 mat, mInv;

    public:

        Transform() :
        mat {Matrix4x4::identity()},
        mInv {Matrix4x4::identity()} {}

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

        /**
         * The translation transform.
         *
         * @param delta a delta vector
         * @return the translation transfom
         * */
        static Transform make_translate(const Vec3& delta) {
            Matrix4x4 mat = {
                {1, 0, 0, delta(0)},
                {0, 1, 0, delta(1)},
                {0, 0, 1, delta(2)},
                {0, 0, 0, 1}
            };
            Matrix4x4 inv = {
                {1, 0, 0, -delta(0)},
                {0, 1, 0, -delta(1)},
                {0, 0, 1, -delta(2)},
                {0, 0, 0, 1}
            };
            return Transform(mat, inv);
        }

        /**
         * Make a scale transformation matrix.
         *
         * @param x
         * @param y
         * @param z
         * @return the scale transform
         * */
        static Transform make_scale(float x, float y, float z) {
            Matrix4x4 mat = {
                {x, 0, 0, 0},
                {0, y, 0, 0},
                {0, 0, z, 0},
                {0, 0, 0, 1}
            };
            Matrix4x4 inv = {
                {1/x, 0, 0, 0},
                {0, 1/y, 0, 0},
                {0, 0, 1/z, 0},
                {0, 0, 0,   1}
            };
            return Transform(mat, inv);
        }

        /**
         * Make a rotation matrix around the X axis.
         *
         * @param theta in degress
         * @return the rotation transform
         * */
        static Transform make_rotateX(float theta) {
            float sin_theta = std::sin(tao::geometry::radians(theta));
            float cos_theta = std::cos(tao::geometry::radians(theta));
            Matrix4x4 m = {
                {1,         0,          0, 0},
                {0, cos_theta, -sin_theta, 0},
                {0, sin_theta, cos_theta,  0},
                {0,         0,          0, 1},
            };
            return Transform(m, m.t());
        }

        /**
         * Make a rotation matrix around the Y axis.
         *
         * @param theta in degress
         * @return the rotation transform
         * */
        static Transform make_rotateY(float theta) {
            float sin_theta = std::sin(tao::geometry::radians(theta));
            float cos_theta = std::cos(tao::geometry::radians(theta));
            Matrix4x4 m = {
                {cos_theta,     0, sin_theta, 0},
                {0,             1,         0, 0},
                {-sin_theta,    0, cos_theta, 0},
                {0,             0,         0, 1},
            };
            return Transform(m, m.t());
        }

        /**
         * Make a rotation matrix around the Z axis.
         *
         * @param theta in degress
         * @return the rotation transform
         * */
        static Transform make_rotateZ(float theta) {
            float sin_theta = std::sin(tao::geometry::radians(theta));
            float cos_theta = std::cos(tao::geometry::radians(theta));
            Matrix4x4 m = {
                {cos_theta, -sin_theta, 0, 0},
                {sin_theta,  cos_theta, 0, 0},
                {        0,          0, 1, 0},
                {        0,          0, 0, 1},
            };
            return Transform(m, m.t());
        }

        /**
         * Make a rotation matrix around an arbitrary axis.
         *
         * @param theta in degress
         * @return the rotation transform
         * */
        static Transform make_rotate(float theta, const Vec3& axis) {
            auto ax = tao::unitize(axis);
            float sin_theta = std::sin(tao::geometry::radians(theta));
            float cos_theta = std::cos(tao::geometry::radians(theta));
            Matrix4x4 mat;
            // first basis vector
            mat(0, 0) = ax(0) * ax(0) + (1 - ax(0) * ax(0)) * cos_theta;
            mat(0, 1) = ax(0) * ax(1) * (1 - cos_theta) - ax(2) * sin_theta;
            mat(0, 2) = ax(0) * ax(2) * (1 - cos_theta) + ax(1) * sin_theta;
            mat(0, 3) = 0;
            // second basis vector
            mat(1, 0) = ax(0) * ax(1) * (1 - cos_theta) + ax(2) * sin_theta;
            mat(1, 1) = ax(1) * ax(1) + (1 - ax(1) * ax(1)) * cos_theta;
            mat(1, 2) = ax(1) * ax(2) * (1 - cos_theta) - ax(0) * sin_theta;
            mat(1, 3) = 0;
            // third basis vector
            mat(2, 0) = ax(0) * ax(2) * (1 - cos_theta) - ax(1) * sin_theta;
            mat(2, 1) = ax(1) * ax(2) * (1 - cos_theta) + ax(0) * sin_theta;
            mat(2, 2) = ax(2) * ax(2) + (1 - ax(2) * ax(2)) * cos_theta;
            mat(2, 3) = 0;
            return Transform(mat, mat.t());
        }

        /**
         * Transform a point.
         *
         * @param a point
         * @return the resulting point
         * */
        Point3 t_point3(const Point3& p) const {
            RealValue x = p(0), y = p(1), z = p(2);
            RealValue xp = mat(0, 0) * x + mat(0, 1) * y + mat(0, 2) * z + mat(0, 3);
            RealValue yp = mat(1, 0) * x + mat(1, 1) * y + mat(1, 2) * z + mat(1, 3);
            RealValue zp = mat(2, 0) * x + mat(2, 1) * y + mat(2, 2) * z + mat(2, 3);
            RealValue wp = mat(3, 0) * x + mat(3, 1) * y + mat(3, 2) * z + mat(3, 3);
            if (wp == 1) return Point3 {xp, yp, zp};
            else return Point3 {xp, yp, zp} / wp;
        }

        /**
         * Transform a vector.
         *
         * @param a vector
         * @return the resulting vector
         * */
        Vec3 t_vec3(const Vec3& v) const {
            RealValue x = v(0), y = v(1), z = v(2);
            return Vec3 {
                mat(0, 0) * x + mat(0, 1) * y + mat(0, 2) * z,
                mat(1, 0) * x + mat(1, 1) * y + mat(1, 2) * z,
                mat(2, 0) * x + mat(2, 1) * y + mat(2, 2) * z,
            };
        }

        /**
         * Transform a normal.
         *
         * @param a normal
         * @return the resulting normal
         * */
        Vec3 t_normal3(const Vec3& n) const {
            RealValue x = n(0), y = n(1), z = n(2);
            return Vec3 {
                mInv(0, 0) * x + mInv(0, 1) * y + mInv(0, 2) * z,
                mInv(1, 0) * x + mInv(1, 1) * y + mInv(1, 2) * z,
                mInv(2, 0) * x + mInv(2, 1) * y + mInv(2, 2) * z,
            };
        }

        /**
         * Transform a ray.
         *
         * @param a ray
         * @return the resulting ray
         * */
        Ray t_ray(const Ray& r) const {
            //TODO deal with round off error
            Point3 o = t_point3(r.get_origin()); 
            Vec3 d = t_vec3(r.get_direction()); 
            return Ray(o, d);
        }

        /**
         * Transform bounds.
         *
         * @param a bounds3
         * @return the resulting bounds
         * */
        Bounds3 t_bounds3(const Bounds3& b) const {
            const Transform& M = *this;
            Bounds3 ret(M.t_point3(Point3{b.pMin(0), b.pMin(1), b.pMin(2)}));
            ret = Bounds3::make_union(ret, M.t_point3(Point3{b.pMax(0), b.pMin(1), b.pMin(2)}));
            ret = Bounds3::make_union(ret, M.t_point3(Point3{b.pMin(0), b.pMax(1), b.pMin(2)}));
            ret = Bounds3::make_union(ret, M.t_point3(Point3{b.pMin(0), b.pMin(1), b.pMax(2)}));
            ret = Bounds3::make_union(ret, M.t_point3(Point3{b.pMin(0), b.pMax(1), b.pMax(2)}));
            ret = Bounds3::make_union(ret, M.t_point3(Point3{b.pMax(0), b.pMax(1), b.pMin(2)}));
            ret = Bounds3::make_union(ret, M.t_point3(Point3{b.pMax(0), b.pMin(1), b.pMax(2)}));
            ret = Bounds3::make_union(ret, M.t_point3(Point3{b.pMax(0), b.pMax(1), b.pMax(2)}));
            return ret;
        }

        /**
         * Transform surface interaction.
         *
         * @param a surface interaction
         * @return the resulting surface interaction
         * */
        SurfaceInteraction t_si(const SurfaceInteraction& b) const {
            const Transform& M = *this;
            SurfaceInteraction si;
            si._p = t_point3(b._p);
            si._n = t_normal3(b._n);
            si._wo = t_vec3(b._wo);
            si._uv = b._uv; //TODO should this be transformed?
            si._t = b._t;
            si._primitive = b._primitive;
            si._time = b._time;
            return si;
        }

        /**
         * Composition of transformations.
         * */
        inline Transform operator*(const Transform& t2) const {
            return Transform {mat * t2.mat, t2.mInv * mInv};
        }

        inline static std::shared_ptr<Transform> compose(const std::vector<std::shared_ptr<Transform>>& ts) {
            Transform transform = Transform(); 
            for (auto t : ts) {
                transform = (*t) * transform;
            }
            return std::make_shared<Transform>(transform);
        }

        bool swap_handedness() const {
            float det = 
                mat(0, 0) * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1)) -
                mat(0, 1) * (mat(1, 0) * mat(2, 2) - mat(1, 2) * mat(2, 0)) +
                mat(0, 2) * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0));
            return det < 0;
        }

        bool operator<(const Transform& t2) const {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    if (mat(i, j) < t2.mat(i, j)) return true;
                    if (mat(i, j) > t2.mat(i, j)) return false;
                }
            }
            return false;
        }
};

};

#endif
