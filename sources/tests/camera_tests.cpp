#include "gtest/gtest.h"
#include "omg/cameras/OrthoCamera.h"
#include "omg/cameras/PerspectiveCamera.h"
#include "data/out_perspcamera_samp_300.cpp"
#include "data/out_orthocamera_samp_300.cpp"

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

    TEST(Camera, OrthoCamera) {
        omg::OrthoCamera camera {
            40,
            30,
            {0.0, 0.0, 0.0},
            {0.0, 0.0, -10.0},
            {0.0, 1.0, 1.0},
            {-3.0, 3.0, -2.25, 2.25}
        }; 

        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 30; ++j) {
                auto p = std::pair{j, i};
                if (out_orthocamera_samp_300.find(p) != out_orthocamera_samp_300.end()) {
                    omg::Ray r = camera.generate_ray(i/40.0, (29-j)/30.0);
                    auto o = r.get_origin(); 
                    ASSERT_TRUE(o.eq(out_orthocamera_samp_300.find(p)->second.get_origin(), 0.01));
                }
            }
        }
    }

    TEST(Camera, PerspCamera) {
        omg::PerspectiveCamera camera {
            40,
            30,
            {0.0, 0.0, 0.0},
            {0.0, 0.0, -10.0},
            {0.0, 1.0, 1.0},
            1.0,
            45.0
        };
    
        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 30; ++j) {
                auto p = std::pair{j, i};
                if (out_perspcamera_samp_300.find(p) != out_perspcamera_samp_300.end()) {
                    omg::Ray r = camera.generate_ray(i/40.0, (29-j)/30.0);
                    auto d = r.get_direction(); 
                    ASSERT_TRUE(d.eq(out_perspcamera_samp_300.find(p)->second.get_direction(), 0.01));
                }
            }
        }
    }

};
