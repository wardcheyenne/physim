#include <gtest/gtest.h>
#include "vector3.h"
using namespace physim;

TEST(Vector3, Addition) {
    Vector3 a(1,2,3), b(4,5,6);
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 5); EXPECT_DOUBLE_EQ(c.y, 7); EXPECT_DOUBLE_EQ(c.z, 9);
}

TEST(Vector3, Norm) {
    Vector3 v(3, 4, 0);
    EXPECT_DOUBLE_EQ(v.norm(), 5.0);
}

TEST(Vector3, DotProduct) {
    EXPECT_DOUBLE_EQ(Vector3(1,0,0).dot(Vector3(0,1,0)), 0.0);
    EXPECT_DOUBLE_EQ(Vector3(1,2,3).dot(Vector3(1,2,3)), 14.0);
}

TEST(Vector3, CrossProduct) {
    auto c = Vector3(1,0,0).cross(Vector3(0,1,0));
    EXPECT_DOUBLE_EQ(c.z, 1.0);
}

TEST(Vector3, NormalizedUnit) {
    Vector3 v = Vector3(3, 4, 0).normalized();
    EXPECT_NEAR(v.norm(), 1.0, 1e-12);
}

TEST(Vector3, NormalizeZeroThrows) {
    EXPECT_THROW(Vector3(0,0,0).normalized(), std::domain_error);
}

TEST(Vector3, ScalarMultiply) {
    Vector3 v = Vector3(1,2,3) * 2;
    EXPECT_DOUBLE_EQ(v.x, 2); EXPECT_DOUBLE_EQ(v.y, 4); EXPECT_DOUBLE_EQ(v.z, 6);
}
