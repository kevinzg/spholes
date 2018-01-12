#ifndef SPHOLES_TEST_LINESEGMENT_H
#define SPHOLES_TEST_LINESEGMENT_H

#include "LineSegment.h"

using spholes::LineSegment;
using spholes::Point;

TEST_CASE("line segment intersection", "[line-segment]")
{
    const LineSegment a(Point(-3.0, -4.0), Point(3.0, 4.0));
    const LineSegment b(a.first + Point(0, 2.0), a.second + Point(0, 2.0));

    const LineSegment c0(a.first * 2.0, a.second * 2.0);
    const LineSegment c1(a.first * 0.5, a.second * 0.5);
    const LineSegment c2(a.first * 2.0, a.second * 0.5);
    const LineSegment c3(a.first * 0.5, a.second * 2.0);

    const LineSegment c4(a.first + a.second * 3.0, a.second * 4.0);

    const LineSegment d(Point(-2.0, 3.0), Point(2.0, -3.0));
    const LineSegment e(d.first + Point(2.0, -3.0), d.second + Point(2.0, -3.0));
    const LineSegment f(e.first + Point(-3.0, -4.0), d.second + Point(-3.0, -4.0));

    SECTION("parallel not intersecting lines")
    {
        Point s, t;

        LineSegment::IntersectionMode intersection = LineSegment::intersection(a, b, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::NoIntersection);
    }

    SECTION("collinear intersecting lines")
    {
        Point s, t;

        LineSegment::IntersectionMode intersection = LineSegment::intersection(a, c0, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::SegmentIntersection);
        REQUIRE(s == a.first);
        REQUIRE(t == a.second);

        intersection = LineSegment::intersection(a, c1, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::SegmentIntersection);
        REQUIRE(s == c1.first);
        REQUIRE(t == c1.second);

        intersection = LineSegment::intersection(a, c2, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::SegmentIntersection);
        REQUIRE(s == a.first);
        REQUIRE(t == c2.second);

        intersection = LineSegment::intersection(a, c3, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::SegmentIntersection);
        REQUIRE(s == c3.first);
        REQUIRE(t == a.second);
    }

    SECTION("collinear not intersecting lines")
    {
        Point s, t;

        LineSegment::IntersectionMode intersection = LineSegment::intersection(a, c4, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::NoIntersection);
    }

    SECTION("non-parallel intersecting lines")
    {
        Point s, t;

        LineSegment::IntersectionMode intersection = LineSegment::intersection(a, d, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::PointIntersection);
        REQUIRE(s == Point());
        REQUIRE(t == Point());

        intersection = LineSegment::intersection(a, e, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::PointIntersection);
        REQUIRE(s == Point());
        REQUIRE(t == Point());

        intersection = LineSegment::intersection(a, f, s, t);

        REQUIRE(intersection == LineSegment::IntersectionMode::PointIntersection);
        REQUIRE(s == a.first);
        REQUIRE(t == f.first);
    }
}

#endif // SPHOLES_TEST_POINT_H
