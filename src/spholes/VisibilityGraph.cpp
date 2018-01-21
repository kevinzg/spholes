#include "VisibilityGraph.h"

#include <algorithm>
#include <set>

#include "LineSegment.h"

namespace spholes {

std::vector<Point> VisibilityGraph::visibleVertices(const Point &pivot, const std::vector<Polygon> &obstacles, PointRef pivotRef)
{
    std::vector<std::pair<PointRef, PolarPoint>> points;

    size_t globalPointCounter = 0;

    for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
    {
        const Polygon &polygon = *it;
        for (auto pointIt = polygon.begin(); pointIt != polygon.end(); ++pointIt)
        {
            PolarPoint polarPoint = Point(*pointIt - pivot).toPolarPoint();
            points.push_back(std::make_pair(PointRef({static_cast<size_t>(std::distance(obstacles.begin(), it)),
                                                      static_cast<size_t>(std::distance(polygon.begin(), pointIt)),
                                                      globalPointCounter++}),
                                            polarPoint));
        }
    }

    if (pivotRef.polygonId == obstacles.size())
        pivotRef.globalPointId = pivotRef.localPointId = globalPointCounter;

    std::sort(points.begin(), points.end(), [](const auto &a, const auto &b) {
        if (a.second.angle() != b.second.angle())
            return a.second.angle() < b.second.angle();

        if (a.second.radius() != b.second.radius())
            return a.second.radius() < b.second.radius();

        return a.first.globalPointId < b.first.globalPointId;
    });

    std::vector<size_t> sortedPointsIndices(points.size());
    for (auto pointIt = points.begin(); pointIt != points.end(); ++pointIt)
        sortedPointsIndices[pointIt->first.globalPointId] = static_cast<size_t>(std::distance(points.begin(), pointIt));

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

        lineSegments[cwLineSegment].lineSegmentId = cwLineSegment;
    }

    const Point origin(0, 0);
    LineSegment sweepLine(origin, Point(INF, 0));

    auto lineSegmentTreeComp = [&points, &sweepLine, &origin](const LineSegmentRef &a, const LineSegmentRef &b)
    {
        LineSegment segmentA(points[a.polarPointAId].second.toPoint(), points[a.polarPointBId].second.toPoint());
        LineSegment segmentB(points[b.polarPointAId].second.toPoint(), points[b.polarPointBId].second.toPoint());

        Point p, q;

        real distanceA = INF;
        real distanceB = INF;

        if (LineSegment::intersection(sweepLine, segmentA, p, q) == LineSegment::PointIntersection)
            distanceA = std::hypot(p.x(), p.y());
        if (LineSegment::intersection(sweepLine, segmentB, p, q) == LineSegment::PointIntersection)
            distanceB = std::hypot(p.x(), p.y());

        if (!approx(distanceA, distanceB))
            return distanceA < distanceB;

        real nextAngle = sweepLine.second.toPolarPoint().angle() + 0.01;
        LineSegment nextSweepLine(origin, PolarPoint(nextAngle, INF).toPoint());

        distanceA = INF;
        distanceB = INF;

        if (LineSegment::intersection(nextSweepLine, segmentA, p, q) == LineSegment::PointIntersection)
            distanceA = std::hypot(p.x(), p.y());
        if (LineSegment::intersection(nextSweepLine, segmentB, p, q) == LineSegment::PointIntersection)
            distanceB = std::hypot(p.x(), p.y());

        if (!approx(distanceA, distanceB))
            return distanceA < distanceB;

        return a.lineSegmentId < b.lineSegmentId;
    };

    typedef std::set<LineSegmentRef, decltype(lineSegmentTreeComp)> LineSegmentTree;
    LineSegmentTree lineSegmentTree(lineSegmentTreeComp);
    std::vector<LineSegmentTree::iterator> lineSegmentIterators(lineSegments.size(), lineSegmentTree.end());

    for (auto lineSegmentIt = lineSegments.begin(); lineSegmentIt != lineSegments.end(); ++lineSegmentIt)
    {
        const Point pointA = points[lineSegmentIt->polarPointAId].second.toPoint();
        const Point pointB = points[lineSegmentIt->polarPointBId].second.toPoint();

        Point p, dummyPointQ;

        LineSegment::IntersectionMode intersection =
                LineSegment::intersection(sweepLine, LineSegment(pointA, pointB), p, dummyPointQ);

        if (intersection != LineSegment::IntersectionMode::PointIntersection)
            continue;

        real cpA = crossProduct(pointA, p);
        real cpB = crossProduct(pointB, p);

        if (cpA * cpB < 0)
        {
            size_t lineSegmentId = static_cast<size_t>(std::distance(lineSegments.begin(), lineSegmentIt));
            lineSegmentIterators[lineSegmentId] = lineSegmentTree.insert(*lineSegmentIt).first;
        }
    }

    std::vector<Point> visiblePoints;

    for (auto pointIt = points.begin(); pointIt != points.end(); ++pointIt)
    {
        const PointRef &pointRef = pointIt->first;
        const PolarPoint &point = pointIt->second;

        if (pointRef.globalPointId == pivotRef.globalPointId)
            continue;

        sweepLine.second = PolarPoint(point.angle(), INF).toPoint();

        auto visible = [&]()
        {
            if (pointRef.polygonId == pivotRef.polygonId)
            {
                size_t pivotNextPoint = pivotRef.localPointId == obstacles[pivotRef.polygonId].size() - 1 ?
                            pivotRef.globalPointId - (obstacles[pivotRef.polygonId].size() - 1) : pivotRef.globalPointId + 1;
                size_t pivotPrevPoint = pivotRef.localPointId == 0 ?
                            pivotRef.globalPointId + (obstacles[pivotRef.polygonId].size() - 1) : pivotRef.globalPointId - 1;

                if (pointRef.globalPointId == pivotNextPoint || pointRef.globalPointId == pivotPrevPoint)
                    return true;

                real prevAngle = points[sortedPointsIndices[pivotPrevPoint]].second.angle();
                real nextAngle = points[sortedPointsIndices[pivotNextPoint]].second.angle();
                real currAngle = point.angle();

                nextAngle -= prevAngle;
                currAngle -= prevAngle;

                nextAngle = nextAngle < 0.0 ? nextAngle + 2 * PI : nextAngle;
                currAngle = currAngle < 0.0 ? currAngle + 2 * PI : currAngle;

                if (nextAngle > currAngle)
                    return false;
            }

            if (pointIt != points.begin() && pointIt->second.angle() == (pointIt - 1)->second.angle())
                return false;

            if (lineSegmentTree.empty())
                return true;

            auto pointAIt = points.begin() + lineSegmentTree.begin()->polarPointAId;
            auto pointBIt = points.begin() + lineSegmentTree.begin()->polarPointBId;

            if (pointAIt == pointIt || pointBIt == pointIt)
                return true;

            LineSegment frontEdge(pointAIt->second.toPoint(), pointBIt->second.toPoint());
            LineSegment visionLine(origin, point.toPoint());

            Point p, dummyPointQ;

            if (LineSegment::intersection(frontEdge, visionLine, p, dummyPointQ) == LineSegment::PointIntersection)
                return approx(p.x(), point.toPoint().x()) && approx(p.y(), point.toPoint().y());

            return true;
        };

        if (visible())
            visiblePoints.push_back(obstacles[pointRef.polygonId][pointRef.localPointId]);

        size_t cwLineSegmentId = pointRef.globalPointId;
        size_t ccwLineSegmentId = pointRef.localPointId > 0 ?
                    pointRef.globalPointId - 1 :
                    pointRef.globalPointId + obstacles[pointRef.polygonId].size() - 1;

        const LineSegmentRef &cwLineSegment = lineSegments[cwLineSegmentId];
        const LineSegmentRef &ccwLineSegment = lineSegments[ccwLineSegmentId];

        auto processLineSegment = [&](const LineSegmentRef& lineSegment) mutable
        {
            if (points[lineSegment.polarPointAId].first.globalPointId == pivotRef.globalPointId ||
                    points[lineSegment.polarPointBId].first.globalPointId == pivotRef.globalPointId)
                return;

            auto &lineSegmentIt = lineSegmentIterators[lineSegment.lineSegmentId];

            Point otherPoint = points[lineSegment.polarPointAId].first.globalPointId == pointRef.globalPointId ?
                        points[lineSegment.polarPointBId].second.toPoint() : points[lineSegment.polarPointAId].second.toPoint();

            real cp = crossProduct(otherPoint, point.toPoint());

            if (cp < 0 && lineSegmentIt == lineSegmentTree.end())
            {
                lineSegmentIt = lineSegmentTree.insert(lineSegment).first;
            }
            else if (cp > 0 && lineSegmentIt != lineSegmentTree.end())
            {
                lineSegmentTree.erase(lineSegmentIt);
                lineSegmentIt = lineSegmentTree.end();
            }
        };

        processLineSegment(cwLineSegment);
        processLineSegment(ccwLineSegment);
    }

    return visiblePoints;
}

void VisibilityGraph::addEdgesToGraph(Graph<Point> &graph, const Point &vertex, const std::vector<Point> &vertices)
{
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
        graph.addEdge(vertex, *it);
}

Graph<Point> VisibilityGraph::find(const Point &start, const Point &destination, const std::vector<Polygon> &obstacles)
{
    Graph<Point> graph;

    PointRef externPoint = { obstacles.size(), 0, 0 };

    addEdgesToGraph(graph, start, visibleVertices(start, obstacles, externPoint));
    addEdgesToGraph(graph, destination, visibleVertices(destination, obstacles, externPoint));

    size_t pointCounter = 0;

    for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
    {
        const Polygon &polygon = *it;
        for (auto point = polygon.begin(); point != polygon.end(); ++point)
        {
            PointRef ref =
            {
                static_cast<size_t>(std::distance(obstacles.begin(), it)),
                static_cast<size_t>(std::distance(polygon.begin(), point)),
                pointCounter++
            };
            addEdgesToGraph(graph, *point, visibleVertices(*point, obstacles, ref));
        }
    }

    return graph;
}

}
