#ifndef SPHOLES_VISIBILITYGRAPH_H
#define SPHOLES_VISIBILITYGRAPH_H

#include <vector>
#include "Graph.h"
#include "Point.h"
#include "Polygon.h"
#include "Hash.h"

namespace spholes {

class VisibilityGraph
{
    struct PointRef;

    static std::vector<Point> visibleVertices(const Point &pivot, const std::vector<Polygon> &obstacles, PointRef pivotRef);
    static void addEdgesToGraph(Graph<Point> &graph, const Point &vertex, const std::vector<Point> &vertices, bool mirror);

    struct PointRef
    {
        size_t polygonId;
        size_t localPointId;
        size_t globalPointId;
    };

    struct LineSegmentRef
    {
        size_t lineSegmentId;
        size_t polarPointAId;
        size_t polarPointBId;
    };

public:
    static Graph<Point> find(const Point &start, const Point &destination, const std::vector<Polygon> &obstacles);
};

}

#endif // SPHOLES_VISIBILITYGRAPH_H
