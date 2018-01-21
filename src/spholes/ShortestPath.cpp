#include "ShortestPath.h"

#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>

namespace spholes {

Path ShortestPath::find(const Point &start, const Point &destination, const Graph<Point> &visibilityGraph)
{
    auto comp = [](const SPNode &a, const SPNode &b)
    {
        if (a.distance != b.distance)
            return a.distance > b.distance;

        return a.point < b.point;
    };

    typedef std::priority_queue<SPNode, std::vector<SPNode>, decltype(comp)> PointQueue;
    PointQueue queue(comp, std::vector<SPNode>());

    queue.push(SPNode {0.0, start, Point(0.0, 0.0)});

    std::map<Point, SPNode> sp;

    while (!queue.empty())
    {
        SPNode top = queue.top();
        queue.pop();

        if (sp.count(top.point))
            continue;

        sp[top.point] = top;

        const auto &edges = visibilityGraph.getEdges(top.point);

        for (auto it = edges.begin(); it != edges.end(); ++it)
        {
            Point diff = top.point - *it;

            if (!sp.count(*it))
                queue.push({ top.distance + std::hypot(diff.x(), diff.y()), *it, top.point });
        }
    }

    Path path;
    Point d = destination;

    while (d != start)
    {
        path.push_back(d);
        d = sp.at(d).parent;
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}

}

