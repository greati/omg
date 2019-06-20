#include "gtest/gtest.h"

#include "omg/raytracer/TransformCache.h"

namespace {

    TEST(TransformCache, Lookup) {
    
        omg::TransformCache tc;

        omg::Transform *t, *ti;

        omg::Transform tid = omg::Matrix4x4::identity();

        bool hasid = false; 
        hasid = tc.lookup(tid, &t, &ti);
        ASSERT_FALSE(hasid);
        ASSERT_TRUE(*t == tid);
        ASSERT_TRUE(*ti == inverse(tid));
        hasid = tc.lookup(tid, &t, &ti);
        ASSERT_TRUE(hasid);
        ASSERT_TRUE(*t == tid);
        ASSERT_TRUE(*ti == inverse(tid));

        omg::Transform mrot = omg::Transform::make_rotateX(90);
        bool hasrot = tc.lookup(mrot, &t, &ti);
        ASSERT_FALSE(hasrot);
        ASSERT_TRUE(*t == mrot);
        ASSERT_TRUE(*ti == inverse(mrot));
        hasrot = tc.lookup(mrot, &t, &ti);
        ASSERT_TRUE(hasrot);
        ASSERT_TRUE(*t == mrot);
        ASSERT_TRUE(*ti == inverse(mrot));
    }


};
