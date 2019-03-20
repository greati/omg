#include "gtest/gtest.h"

#include "math/Vec.h"

namespace rayt {

    TEST(Vec3Test, CreationBinding) {
        Vec3<> v {0.1, 0.2, 0.3};

        auto [x, y, z] = v();

        ASSERT_FLOAT_EQ(x, 0.1);
        ASSERT_FLOAT_EQ(y, 0.2);
        ASSERT_FLOAT_EQ(z, 0.3);
    }

    TEST(Vec3Test, Creation) {

        Vec3<> v {0.1, 0.2, 0.3};

        auto [x, y, z] = v();

        auto [x1, y1, z1] = v();
        
        ASSERT_FLOAT_EQ(x, 0.1);
        ASSERT_FLOAT_EQ(y, 0.2);
        ASSERT_FLOAT_EQ(z, 0.3);
        ASSERT_FLOAT_EQ(x1, 0.1);
        ASSERT_FLOAT_EQ(y1, 0.2);
        ASSERT_FLOAT_EQ(z1, 0.3);
        ASSERT_TRUE(x1==x and y1==y and z1==z);
    }

    TEST(Vec3Test, CopyAssignment) {
        Vec3<> v {0.1, 0.2, 0.3};
        auto v2 = v;

        auto [x, y, z] = v2();
        auto [x1, y1, z1] = v();

        ASSERT_FLOAT_EQ(x, 0.1);
        ASSERT_FLOAT_EQ(y, 0.2);
        ASSERT_FLOAT_EQ(z, 0.3);
        ASSERT_FLOAT_EQ(x1, 0.1);
        ASSERT_FLOAT_EQ(y1, 0.2);
        ASSERT_FLOAT_EQ(z1, 0.3);
    }

    TEST(Vec3Test, InitListAssignment) {
        Vec3<> v {5.0, 6.0, 7.0};
        
        v = {1.0, 2.0, 3.0};

        auto [x, y, z] = v();

        ASSERT_FLOAT_EQ(x, 1.0);
        ASSERT_FLOAT_EQ(y, 2.0);
        ASSERT_FLOAT_EQ(z, 3.0);
    }
};
