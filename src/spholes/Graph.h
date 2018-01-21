#ifndef SPHOLES_GRAPH_H
#define SPHOLES_GRAPH_H

#include <map>
#include <vector>

namespace spholes {

template <typename V>
class Graph
{
    std::map<V, std::vector<V>> adjacencyLists;

public:
    Graph();

    void addEdge(const V &a, const V &b);
    inline auto begin() const { return adjacencyLists.begin(); }
    inline auto end() const { return adjacencyLists.end(); }
    inline const auto & getEdges(const V& p) const { return adjacencyLists.at(p); }
};

template <typename V>
Graph<V>::Graph()
{
}

template<typename V>
void Graph<V>::addEdge(const V &a, const V &b)
{
    adjacencyLists[a].push_back(b);
}

}

#endif // SPHOLES_GRAPH_H
