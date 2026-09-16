#include <vector>
#include <deque>
#include <limits> // Required for std::numeric_limits

class Solution {
public:
    // Renamed the function to match the driver code's expectation: 'minimumEdgeReversal'.
    // Also, adjusted parameter order and removed 'm' as it's not passed by the driver.
    int minimumEdgeReversal(std::vector<std::vector<int>>& edges, int n, int src, int dst) {
        // Adjacency list to represent the graph.
        // For each node u, adj[u] will store pairs {v, weight}.
        // weight = 0 if the edge u -> v is used in its original direction.
        // weight = 1 if the edge v -> u is used (meaning the original edge u -> v was reversed).
        // Vertices are 1-indexed, so we use n+1 size for adjacency list and distance array.
        std::vector<std::vector<std::pair<int, int>>> adj(n + 1);

        // Build the graph with weighted edges.
        for (const auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            
            // Original edge u -> v costs 0 reversals.
            adj[u].push_back({v, 0});
            
            // To allow a path v -> u, we consider reversing the edge u -> v.
            // This reversed edge v -> u costs 1 reversal.
            adj[v].push_back({u, 1});
        }

        // Distance array to store the minimum number of reversals to reach each node from src.
        // Initialize all distances to a very large value (infinity).
        std::vector<int> dist(n + 1, std::numeric_limits<int>::max());

        // Deque for 0-1 BFS.
        // Nodes with 0-cost edges are pushed to the front.
        // Nodes with 1-cost edges are pushed to the back.
        std::deque<int> dq;

        // Start BFS from the source vertex.
        dist[src] = 0;
        dq.push_front(src);

        while (!dq.empty()) {
            int u = dq.front();
            dq.pop_front();

            // Iterate over all neighbors of u.
            for (const auto& edge_info : adj[u]) {
                int v = edge_info.first;
                int weight = edge_info.second; // 0 for original, 1 for reversed

                // If a shorter path to v is found through u.
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    // If the edge has weight 0, push to front (higher priority).
                    if (weight == 0) {
                        dq.push_front(v);
                    } 
                    // If the edge has weight 1, push to back (lower priority).
                    else { // weight == 1
                        dq.push_back(v);
                    }
                }
            }
        }

        // After the BFS, dist[dst] will hold the minimum reversals.
        // If dst is unreachable, dist[dst] will still be std::numeric_limits<int>::max().
        if (dist[dst] == std::numeric_limits<int>::max()) {
            return -1; // Path not possible
        } else {
            return dist[dst]; // Minimum reversals required
        }
    }
};