#ifndef SPHOLES_SHORTESTPATH_H
#define SPHOLES_SHORTESTPATH_H

#include <vector>
#include "Common.h"
#include "Point.h"
#include "Polygon.h"
#include "Path.h"
#include "Graph.h"

namespace spholes {

class ShortestPath
{
    struct SPNode
    {
        real distance;
        Point point;
        Point parent;
    };

public:
    static Path find(const Point &start, const Point &destination, const Graph<Point> &visibilityGraph);
};

}

#endif // SPHOLES_SHORTESTPATH_H
