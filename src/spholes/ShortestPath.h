#ifndef SPHOLES_SHORTESTPATH_H
#define SPHOLES_SHORTESTPATH_H

#include <vector>
#include "Common.h"
#include "Point.h"
#include "Polygon.h"
#include "Path.h"

namespace spholes {

class ShortestPath
{
public:
    static Path find(const Point &start, const Point &destination, const std::vector<Polygon> &obstacles);
};

}

#endif // SPHOLES_SHORTESTPATH_H
