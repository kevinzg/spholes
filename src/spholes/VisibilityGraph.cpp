#include "VisibilityGraph.h"

#include <algorithm>
#include <set>

#include "LineSegment.h"

namespace spholes {

std::vector<Point> VisibilityGraph::visibleVertices(const Point &pivot, const std::vector<Polygon> &obstacles, PointRef pivotRef)
{
    // Vector to store the points.
    std::vector<std::pair<PointRef, PolarPoint>> points;

    // Counter of all the vertices in the obstacles.
    size_t globalPointCounter = 0;

    // Iterate on all the obstacles and their vertices to add them to the _points_ vector.
    for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
    {
        const Polygon &polygon = *it;
        for (auto pointIt = polygon.begin(); pointIt != polygon.end(); ++pointIt)
        {
            // We add the points in polar coordinates, translating them so _pivot_ becomes the origin.
            PolarPoint polarPoint = Point(*pointIt - pivot).toPolarPoint();
            points.push_back(std::make_pair(PointRef({static_cast<size_t>(std::distance(obstacles.begin(), it)),
                                                      static_cast<size_t>(std::distance(polygon.begin(), pointIt)),
                                                      globalPointCounter++}),
                                            polarPoint));
        }
    }

    // If the pivot is an extern point (i.e. not part of the obstacles) update its reference so calculations are easier later on.
    if (pivotRef.polygonId == obstacles.size())
        pivotRef.globalPointId = pivotRef.localPointId = globalPointCounter;

    // Sort the points by their angle, then radius.
    std::sort(points.begin(), points.end(), [](const auto &a, const auto &b) {
        if (a.second.angle() != b.second.angle())
            return a.second.angle() < b.second.angle();

        if (a.second.radius() != b.second.radius())
            return a.second.radius() < b.second.radius();

        return a.first.globalPointId < b.first.globalPointId;
    });

    // This is an index to find a point on the sorted vector by its global point id.
    std::vector<size_t> sortedPointsIndices(points.size());
    for (auto pointIt = points.begin(); pointIt != points.end(); ++pointIt)
        sortedPointsIndices[pointIt->first.globalPointId] = static_cast<size_t>(std::distance(points.begin(), pointIt));

    // Vector to store references to line segments.
    std::vector<LineSegmentRef> lineSegments(points.size());

    // Build the _lineSegments_ vector iterating over all points.
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

    // From now on the pivot will be the origin.
    const Point origin(0, 0);
    LineSegment sweepLine(origin, Point(INF, 0));

    // Line segment comparator based on the distance from the pivot to each line segment throug the sweep line.
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

        // If the distances are the same (e.g. the line segments share a vertex on the sweep line)
        // advance the sweep line a little bit and compare again.
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

    // Line segment tree to store the line segments using the previous comparator.
    typedef std::set<LineSegmentRef, decltype(lineSegmentTreeComp)> LineSegmentTree;
    LineSegmentTree lineSegmentTree(lineSegmentTreeComp);
    std::vector<LineSegmentTree::iterator> lineSegmentIterators(lineSegments.size(), lineSegmentTree.end());

    // Add line segments that intersect the current sweep line.
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

        // This means the line segment properly intersects the sweep line, so it has to be added to the tree.
        if (cpA * cpB < 0)
        {
            size_t lineSegmentId = static_cast<size_t>(std::distance(lineSegments.begin(), lineSegmentIt));
            lineSegmentIterators[lineSegmentId] = lineSegmentTree.insert(*lineSegmentIt).first;
        }
    }

    // Vector to store the visible points, it's the vector to be returned at the end.
    std::vector<Point> visiblePoints;

    // Iterate on all points to check if they are visible.
    for (auto pointIt = points.begin(); pointIt != points.end(); ++pointIt)
    {
        const PointRef &pointRef = pointIt->first;
        const PolarPoint &point = pointIt->second;

        // Skip pivot.
        if (pointRef.globalPointId == pivotRef.globalPointId)
            continue;

        // Move _sweepLine_ to be the ray from the origin to _point_.
        sweepLine.second = PolarPoint(point.angle(), INF).toPoint();

        // This determines if the point is visible or not.
        auto visible = [&]()
        {
            // Do a special check if _point_ and the pivot are vertices of the same polygon.
            if (pointRef.polygonId == pivotRef.polygonId)
            {
                // Next and previous points to the pivot. Clockwise.
                size_t pivotNextPoint = pivotRef.localPointId == obstacles[pivotRef.polygonId].size() - 1 ?
                            pivotRef.globalPointId - (obstacles[pivotRef.polygonId].size() - 1) : pivotRef.globalPointId + 1;
                size_t pivotPrevPoint = pivotRef.localPointId == 0 ?
                            pivotRef.globalPointId + (obstacles[pivotRef.polygonId].size() - 1) : pivotRef.globalPointId - 1;

                // If the two vertices are adyacent then the point is visble.
                if (pointRef.globalPointId == pivotNextPoint || pointRef.globalPointId == pivotPrevPoint)
                    return true;

                // If the point is between the two incident edges to the pivot then the point is not visible.
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

            // If the point before this one shares the same angle with this (i.e. both are on the sweep line) then return false.
            // Note that the point might be visible, but for the shortest path problem this isn't really important since the previous point
            // will be connected to this one.
            // The right thing to do here would be to check if there is an edge on the line segments tree between
            // the previous point radius and this point radius.
            if (pointIt != points.begin() && pointIt->second.angle() == (pointIt - 1)->second.angle())
                return false;

            // If there are no edges on the line segments tree then the point is visible
            if (lineSegmentTree.empty())
                return true;

            // These are the points of the first line segment in the line segment tree.
            auto pointAIt = points.begin() + lineSegmentTree.begin()->polarPointAId;
            auto pointBIt = points.begin() + lineSegmentTree.begin()->polarPointBId;

            // If the point is one of the front edge ones then mark it as visible.
            if (pointAIt == pointIt || pointBIt == pointIt)
                return true;

            // Just check if the front edge on the tree intersects the vision line from the origin to the point.
            LineSegment frontEdge(pointAIt->second.toPoint(), pointBIt->second.toPoint());
            LineSegment visionLine(origin, point.toPoint());

            Point p, dummyPointQ;

            // If the point of intersection is _point_ then it is visible.
            if (LineSegment::intersection(frontEdge, visionLine, p, dummyPointQ) == LineSegment::PointIntersection)
                return approx(p.x(), point.toPoint().x()) && approx(p.y(), point.toPoint().y());

            return true;
        };

        // Check if the point is visible.
        if (visible())
            visiblePoints.push_back(obstacles[pointRef.polygonId][pointRef.localPointId]);

        // Find the line segments incident to _point_.
        size_t cwLineSegmentId = pointRef.globalPointId;
        size_t ccwLineSegmentId = pointRef.localPointId > 0 ?
                    pointRef.globalPointId - 1 :
                    pointRef.globalPointId + obstacles[pointRef.polygonId].size() - 1;

        const LineSegmentRef &cwLineSegment = lineSegments[cwLineSegmentId];
        const LineSegmentRef &ccwLineSegment = lineSegments[ccwLineSegmentId];

        // This adds or removes the line segment to the line segment tree.
        auto processLineSegment = [&](const LineSegmentRef& lineSegment) mutable
        {
            // If the line segment is incident to the pivot then do nothing.
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

void VisibilityGraph::addEdgesToGraph(Graph<Point> &graph, const Point &vertex, const std::vector<Point> &vertices, bool mirror = false)
{
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        graph.addEdge(vertex, *it);
        if (mirror)
            graph.addEdge(*it, vertex);
    }
}

Graph<Point> VisibilityGraph::find(const Point &start, const Point &destination, const std::vector<Polygon> &obstacles)
{
    Graph<Point> graph;

    PointRef externPoint = { obstacles.size(), 0, 0 };

    addEdgesToGraph(graph, start, visibleVertices(start, obstacles, externPoint), true);
    addEdgesToGraph(graph, destination, visibleVertices(destination, obstacles, externPoint), true);

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
