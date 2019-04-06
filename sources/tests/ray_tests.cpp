#include "gtest/gtest.h"
#include "omg/raytracer/Ray.h"

namespace {


    TEST(Ray, RayCreation) {
        omg::Ray r {{0.0, 0.0, 0.0}, {1.0, 0.0, 1.0}}; 
        ASSERT_TRUE((r.get_origin()) == (omg::Point3{0.0, 0.0, 0.0}));
        ASSERT_TRUE((r.get_direction()) == (omg::Vec3{1.0, 0.0, 1.0}));
    }

    TEST(Ray, PointInRay) {
        omg::Ray r {{0.0, 1.0, 0.0}, {1.0, 0.0, 1.0}}; 
        ASSERT_TRUE(r(1.2) == (omg::Point3{1.2, 1.0, 1.2}));
        ASSERT_TRUE(r(0.5) == (omg::Point3{0.5, 1.0, 0.5}));
    }




};
