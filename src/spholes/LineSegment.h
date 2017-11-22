#ifndef SPHOLES_LINESEGMENT_H
#define SPHOLES_LINESEGMENT_H

#include <utility>
#include "Common.h"
#include "Point.h"

namespace spholes {

class LineSegment : public std::pair<Point, Point>
{
public:
    LineSegment() {}
    LineSegment(const Point &a, const Point &b) : std::pair<Point, Point> (a, b) {}


    static bool intersection(const LineSegment &a, const LineSegment &b, Point &p, Point &q)
    {
        Point dirA = a.second - a.first;
        Point dirB = b.second - b.first;

        Point dirAB = b.first - a.first;

        real crossProductAB = crossProduct(dirA, dirB);
        real crossProductABB = crossProduct(dirAB, dirB);

        if (approx(crossProductAB, 0.0) && approx(crossProductABB, 0.0))
        {
            Point x = dirA / dotProduct(dirA, dirA);
            real s = dotProduct(dirAB, x);
            real t = s + dotProduct(dirB, x);

            if (s > t) std::swap(s, t);

            s = std::max(s, 0.0);
            t = std::min(t, 1.0);

            if (s > 1.0 || t < 0.0)
                return false;

            p = a.first + dirA * s;
            q = a.first + dirA * t;

            return true;
        }
        if (approx(crossProductAB, 0.0))
        {
            return false;
        }

        real s = crossProduct(dirAB, dirB) / crossProductAB;
        real t = crossProduct(dirAB, dirA) / crossProductAB;

        if (s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0)
        {
            p = a.first + dirA * s;
            q = b.first + dirB * t;

            return true;
        }

        return false;
    }
};

}

#endif // SPHOLES_LINESEGMENT_H
