#ifndef _OMG_B3_
#define _OMG_B3_

#include "omg/common.h"

namespace omg {
class Ray;
class Bounds3 {

    public:

        Point3 pMin, pMax;

        Bounds3();

        Bounds3(const Point3& p);

        Bounds3(const Point3& p1, const Point3& p2);

        const Point3& operator[](int i) const;

        Point3& operator[](int i);

        Point3 corner(int c) const;

        Bounds3 get_union(const Bounds3& b, const Point3& p);

        Bounds3 get_union(const Bounds3& b1, const Bounds3& b2);

        static Bounds3 make_union(const Bounds3& b1, const Bounds3& b2);

        Bounds3 get_intersection(const Bounds3& b1, const Bounds3& b2);

        bool overlaps(const Bounds3& b1, const Bounds3& b2);

        bool inside(const Point3& p, const Bounds3& b) const;

        bool inside_exclusive(const Point3& p, const Bounds3& b);

        Bounds3 expand(const Bounds3& b, RealValue delta);

        Vec3 diagonal() const;

        RealValue surface_area() const;

        RealValue volume() const;

        int maximum_extent() const;

        /*Point3 lerp(const Point3& t) const {
            return Point3 {
                tao::lerp(t(0), pMin(0), pMax(0)),
                tao::lerp(t(1), pMin(1), pMax(1)),
                tao::lerp(t(2), pMin(2), pMax(2))
            };
        }*/
        
        Vec3 offset(const Point3& p) const;

        void bounding_sphere(Point3 * center, float * radius) const;

        bool intersect(const Ray& ray, float *hitt0, float* hitt1) const;

        bool intersect(const Ray& ray, const Vec3 &inv_dir, 
                const int dir_is_neg[3]) const;
};
};
#endif
