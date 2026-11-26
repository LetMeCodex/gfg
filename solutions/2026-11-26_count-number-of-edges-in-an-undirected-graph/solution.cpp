#include <vector>
#include <numeric>

class Solution {
public:
    /**
     * Counts the total number of edges in an undirected graph represented by an adjacency list.
     *
     * @param adj The adjacency list representation of the graph. adj[i] contains a list of vertices adjacent to vertex i.
     * @return The total number of edges in the graph.
     */
    int countEdges(std::vector<std::vector<int>>& adj) {
        int total_degree = 0;
        for (const auto& neighbors : adj) {
            total_degree += neighbors.size();
        }
        // In an undirected graph, the sum of degrees of all vertices is twice the number of edges.
        return total_degree / 2;
    }
};