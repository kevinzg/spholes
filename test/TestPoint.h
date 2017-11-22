#ifndef SPHOLES_TEST_POINT_H
#define SPHOLES_TEST_POINT_H

#include "Point.h"

TEST_CASE("point operations", "[point]")
{
    const spholes::Point a;
    const spholes::Point b(1.618, 3.141);
    const spholes::Point c(b);

    SECTION("new point")
    {
        REQUIRE(a.x() == 0);
        REQUIRE(a.y() == 0);

        REQUIRE(b.x() == 1.618);
        REQUIRE(b.y() == 3.141);

        REQUIRE(c == b);
    }

    SECTION("point addition")
    {
        spholes::Point d = a + b;
        REQUIRE(d == b);

        spholes::Point e = d + b;
        REQUIRE(e.x() == d.x() + b.x());
        REQUIRE(e.y() == d.y() + b.y());
    }

    SECTION("point substraction")
    {
        spholes::Point d = a - b;
        REQUIRE(d == -b);

        spholes::Point e = d - b;
        REQUIRE(e.x() == d.x() - b.x());
        REQUIRE(e.y() == d.y() - b.y());
    }

    SECTION("scalar multiplication")
    {
        spholes::Point d = 2.0 * b;
        REQUIRE(d.x() == 2 * b.x());
        REQUIRE(d.y() == 2 * b.y());

        spholes::Point e = -10.0 * d;
        REQUIRE(e.x() == -10 * d.x());
        REQUIRE(e.y() == -10 * d.y());
    }
}

#endif // SPHOLES_TEST_POINT_H
