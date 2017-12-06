#include "pointcloud.h"
#include <algorithm>
#include "boost/graph/graph_traits.hpp"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "boost/graph/adjacency_list.hpp"

using namespace boost;

//vecS is an alias of std::vector
//order of types: OutEdgeList, VertexList, Directed, VertexProperties, EdgeProperties, GraphProperties, EdgeList
typedef adjacency_list < vecS, vecS, undirectedS,
  property<vertex_distance_t, size_t>, property < edge_weight_t, float > > Graph;

typedef std::pair<size_t, size_t> Edge;
typedef adjacency_list < listS, vecS, directedS,
  no_property, property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;

PointCloud::PointCloud(const std::vector<glm::vec3> &points)
{
    m_points.assign(points.begin(),points.end());
}

void PointCloud::resortPoints()
{
    std::stable_sort(m_points.begin(),m_points.end(),CompareVec3());
}

void PointCloud::generateEdges(std::vector<std::vector<size_t>> &directedEdges)
{
    this->resortPoints();

    const size_t num_nodes = m_points.size();
    std::vector<Edge> edges = std::vector<Edge>();
    std::vector<float> weights = std::vector<float>();
    float farthest = 0.f;
    for(auto it = m_points.begin(); it != m_points.end(); ++it)
    {
        const glm::vec3 v = *it;
        farthest = glm::max(farthest, glm::length(v));
    }
    farthest = farthest * .1f;
    for(size_t i = 0; i < num_nodes; i++)
    {
        for(size_t j = i+1; j < num_nodes; j++)
        {
            edges.push_back(Edge(i,j));
            weights.push_back(approximateCost(i,j, farthest));
        }

    }
    Graph g(&edges.front(), &edges.back(), &weights.front(), num_nodes);
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));
    vertex_descriptor s = vertex(0, g);

    dijkstra_shortest_paths(g, s,
                            predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                            distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));
    directedEdges.resize(num_nodes);
    for (std::size_t i = 0; i != p.size(); ++i)
    {
        if (p[i] != i)
        {
            directedEdges[p[i]].push_back(i);
        }
    }
}

const glm::vec3 origin = glm::vec3(0.f);

float PointCloud::approximateCost(const size_t i0, const size_t i1, const float weight)
{
    const glm::vec3 a = m_points[i0];
    const glm::vec3 b = m_points[i1];
    const float distance = glm::distance2(a,b);
    if(!i0 || !i1)
    {
        return distance;
    }
    return distance + weight * glm::distance2(origin,glm::normalize(b));
}
