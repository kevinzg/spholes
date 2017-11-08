#ifndef SPHOLES_LINESEGMENT_H
#define SPHOLES_LINESEGMENT_H

#include <utility>
#include "Common.h"
#include "Point.h"

namespace spholes {

class LineSegment : public std::pair<Point, Point>
{
};

}

#endif // SPHOLES_LINESEGMENT_H
