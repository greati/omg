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
};
};
#endif
