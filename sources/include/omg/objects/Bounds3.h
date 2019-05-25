#ifndef _OMG_B3_
#define _OMG_B3_

#include "omg/common.h"

namespace omg {
class Bounds3 {

    public:

        Point3 pMin, pMax;

        Bounds3() {
            RealValue minNum = std::numeric_limits<RealValue>::lowest();
            RealValue maxNum = std::numeric_limits<RealValue>::max();
            pMin = Point3 {maxNum, maxNum, maxNum};
            pMax = Point3 {minNum, minNum, minNum};
        }

        Bounds3(const Point3& p)
            : pMin {p}, pMax {p} {}

        Bounds3(const Point3& p1, const Point3& p2) 
            : pMin {Point3 {
                    std::min(p1(0), p2(0)), 
                    std::min(p1(1), p2(1)),
                    std::min(p1(2), p2(2))}},
              pMax {Point3 {
                    std::max(p1(0), p2(0)), 
                    std::max(p1(1), p2(1)),
                    std::max(p1(2), p2(2))}} {}

        const Point3& operator[](int i) const {
            if (i == 0) return pMin;
            if (i == 1) return pMax;
        }

        Point3& operator[](int i) {
            if (i == 0) return pMin;
            if (i == 1) return pMax;
        }

        Point3 corner(int c) const {
            return Point3{
                (*this)[(c & 1)](0),
                (*this)[(c & 2)](1),
                (*this)[(c & 4)](2)
            };
        }

        Bounds3 get_union(const Bounds3& b, const Point3& p) {
            return Bounds3 {
                Point3 {
                    std::min(b.pMin(0), p(0)),
                    std::min(b.pMin(1), p(1)),
                    std::min(b.pMin(2), p(2))
                },
                Point3 {
                    std::max(b.pMax(0), p(0)),
                    std::max(b.pMax(1), p(1)),
                    std::max(b.pMax(2), p(2))
                },
            };
        };

        Bounds3 get_union(const Bounds3& b1, const Bounds3& b2) {
            return Bounds3 {
                Point3 {
                    std::min(b1.pMin(0), b2.pMin(0)),
                    std::min(b1.pMin(1), b2.pMin(1)),
                    std::min(b1.pMin(2), b2.pMin(2))
                },
                Point3 {
                    std::max(b1.pMax(0), b2.pMax(0)),
                    std::max(b1.pMax(1), b2.pMax(1)),
                    std::max(b1.pMax(2), b2.pMax(2))
                },
            };
        };

        Bounds3 get_intersection(const Bounds3& b1, const Bounds3& b2) {
             return Bounds3 {
                Point3 {
                    std::max(b1.pMin(0), b2.pMin(0)),
                    std::max(b1.pMin(1), b2.pMin(1)),
                    std::max(b1.pMin(2), b2.pMin(2))
                },
                Point3 {
                    std::min(b1.pMax(0), b2.pMax(0)),
                    std::min(b1.pMax(1), b2.pMax(1)),
                    std::min(b1.pMax(2), b2.pMax(2))
                },
            };        
        }

        bool overlaps(const Bounds3& b1, const Bounds3& b2) {
            bool x = (b1.pMax(0) >= b2.pMin(0)) && (b1.pMin(0) <= b2.pMax(0));
            bool y = (b1.pMax(1) >= b2.pMin(1)) && (b1.pMin(1) <= b2.pMax(1));
            bool z = (b1.pMax(2) >= b2.pMin(2)) && (b1.pMin(2) <= b2.pMax(2));
            return (x && y && z);
        }

        bool inside(const Point3& p, const Bounds3& b) const {
            return (p(0) >= b.pMin(0) && p(0) <= b.pMax(0) &&
                    p(1) >= b.pMin(1) && p(1) <= b.pMax(1) &&
                    p(2) >= b.pMin(2) && p(2) <= b.pMax(2));
        }

        bool inside_exclusive(const Point3& p, const Bounds3& b) {
            return (p(0) >= b.pMin(0) && p(0) < b.pMax(0) &&
                    p(1) >= b.pMin(1) && p(1) < b.pMax(1) &&
                    p(2) >= b.pMin(2) && p(2) < b.pMax(2));
        }

        Bounds3 expand(const Bounds3& b, RealValue delta) {
            return Bounds3(
                    b.pMin - Vec3{delta, delta, delta},
                    b.pMax + Vec3{delta, delta, delta});
        }

        inline Vec3 diagonal() const { return pMax - pMin; }

        RealValue surface_area() const {
            Vec3 d = diagonal();
            return 2 * (d(0) * d(1) + d(0) * d(2) + d(1) * d(2));
        }

        RealValue volume() const {
            Vec3 d = diagonal();
            return d(0) * d(1) * d(2); 
        }

        int maximum_extent() const {
            Vec3 d = diagonal();
            if (d(0) > d(1) && d(0) > d(2)) {
                return 0;
            } else if (d(1) > d(2)) {
                return 1;
            } else {
                return 2; 
            }
        }

        /*Point3 lerp(const Point3& t) const {
            return Point3 {
                tao::lerp(t(0), pMin(0), pMax(0)),
                tao::lerp(t(1), pMin(1), pMax(1)),
                tao::lerp(t(2), pMin(2), pMax(2))
            };
        }*/
        
        Vec3 offset(const Point3& p) const {
            Vec3 o = p - pMin;
            if (pMax(0) > pMin(0)) o(0) /= pMax(0) - pMin(0);
            if (pMax(1) > pMin(1)) o(1) /= pMax(1) - pMin(1);
            if (pMax(2) > pMin(2)) o(2) /= pMax(2) - pMin(2);
            return o;
        } 

        void bounding_sphere(Point3 * center, float * radius) const {
            *center = (pMin + pMax) / 2;
            *radius = inside(*center, *this) ? distance(*center, pMax) : 0;
        }
};
};
#endif
