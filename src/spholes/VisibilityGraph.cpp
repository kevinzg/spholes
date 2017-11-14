#include "VisibilityGraph.h"

namespace spholes {

std::vector<Point> VisibilityGraph::visibleVertices(const Point &p, const std::vector<Polygon> &obstacles)
{
    return std::vector<Point>();
}

void VisibilityGraph::addEdgesToGraph(Graph<Point> &graph, const Point &vertex, const std::vector<Point> &vertices)
{
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
        graph.addEdge(vertex, *it);
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
