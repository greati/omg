#include "gtest/gtest.h"
#include "omg/cameras/OrthoCamera.h"

namespace {


    TEST(Camera, OrthoBasis) {

        omg::OrthoCamera camera {
           200,
           100,
           {0.0, 0.0, 0.0},
           {0.0, 0.0, -1.0},
           {0.0, 1.0, 0.0},
           {-1.0, 1.0, 1.0, -1.0}
        };

        auto [u, v, w] = camera.get_ortho_basis();

        ASSERT_TRUE(w == (omg::Vec3{0.0, 0.0, 1.0}));
        ASSERT_TRUE(v == (omg::Vec3{0.0, 1.0, 0.0}));
        ASSERT_TRUE(u == (omg::Vec3{1.0, 0.0, 0.0}));
    }

};
