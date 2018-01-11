#include "VisibilityGraph.h"

#include <algorithm>

namespace spholes {

std::vector<Point> VisibilityGraph::visibleVertices(const Point &p, const std::vector<Polygon> &obstacles)
{
    std::vector<std::pair<PointRef, PolarPoint>> points;

    size_t globalPointCounter = 0;

    for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
    {
        const Polygon &polygon = *it;
        for (auto pointIt = polygon.begin(); pointIt != polygon.end(); ++pointIt)
        {
            points.push_back(std::make_pair(PointRef({static_cast<size_t>(std::distance(obstacles.begin(), it)),
                                                      static_cast<size_t>(std::distance(polygon.begin(), pointIt)),
                                                      globalPointCounter++}),
                                            makePolarPoint(p, *pointIt)));
        }
    }

    std::sort(points.begin(), points.end(), [](const auto &a, const auto &b) {
        if (a.second.angle() != b.second.angle())
            return a.second.angle() < b.second.angle();

        if (a.second.radius() != b.second.radius())
            return a.second.radius() < b.second.radius();

        return a.first.globalPointId < b.first.globalPointId;
    });

    std::vector<LineSegmentRef> lineSegments(points.size());

    for (auto pointIt = points.begin(); pointIt != points.end(); ++pointIt)
    {
        size_t polarPointId = static_cast<size_t>(std::distance(points.begin(), pointIt));

        size_t cwLineSegment = pointIt->first.globalPointId;
        size_t ccwLineSegment = pointIt->first.localPointId > 0 ?
                    pointIt->first.globalPointId - 1 :
                    pointIt->first.globalPointId + obstacles[pointIt->first.polygonId].size() - 1;

        lineSegments[cwLineSegment].polarPointAId = polarPointId;
        lineSegments[ccwLineSegment].polarPointBId = polarPointId;
    }

    return std::vector<Point>();
}

void VisibilityGraph::addEdgesToGraph(Graph<Point> &graph, const Point &vertex, const std::vector<Point> &vertices)
{
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
        graph.addEdge(vertex, *it);
}

PolarPoint VisibilityGraph::makePolarPoint(const Point &center, const Point &point)
{
    Point translatedPoint = point - center;

    return PolarPoint(std::atan2(translatedPoint.y(), translatedPoint.x()),
                      std::hypot(translatedPoint.x(), translatedPoint.y()));
}

Graph<Point> VisibilityGraph::find(const Point &start, const Point &destination, const std::vector<Polygon> &obstacles)
{
    Graph<Point> graph;

    addEdgesToGraph(graph, start, visibleVertices(start, obstacles));
    addEdgesToGraph(graph, destination, visibleVertices(destination, obstacles));

    for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
    {
        const Polygon &polygon = *it;
        for (auto point = polygon.begin(); point != polygon.end(); ++point)
            addEdgesToGraph(graph, *point, visibleVertices(*point, obstacles));
    }

    return graph;
}

}
