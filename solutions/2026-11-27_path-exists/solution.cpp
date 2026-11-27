#include <vector>
#include <queue>

class Solution {
public:
    /**
     * @brief Checks if a path exists between two vertices in an undirected graph.
     *
     * @param adj The adjacency list representation of the graph.
     * @param u The starting vertex.
     * @param v The destination vertex.
     * @return true if a path exists, false otherwise.
     */
    bool checkPath(std::vector<std::vector<int>>& adj, int u, int v) {
        int V = adj.size();
        std::vector<bool> visited(V, false);
        std::queue<int> q;

        q.push(u);
        visited[u] = true;

        while (!q.empty()) {
            int current_vertex = q.front();
            q.pop();

            if (current_vertex == v) {
                return true;
            }

            for (int neighbor : adj[current_vertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        return false;
    }
};