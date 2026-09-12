#include <vector>
#include <queue>
#include <algorithm> // Not strictly needed for this specific implementation, but generally useful

class Solution {
public:
    // Helper function to perform BFS starting from a given node.
    // It returns a pair: {farthest_node_index, max_distance_from_start_node}.
    // Nodes are 0-indexed internally.
    std::pair<int, int> bfs(int start_node, int n, const std::vector<std::vector<int>>& adj_list) {
        // dist[i] stores the distance from start_node to node i.
        // -1 indicates an unvisited node.
        std::vector<int> dist(n, -1);
        std::queue<int> q;

        // Initialize BFS from the start_node.
        q.push(start_node);
        dist[start_node] = 0;

        int farthest_node = start_node;
        int max_dist = 0;

        // Standard BFS traversal.
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // Update farthest node and max distance found so far.
            // If multiple nodes are at the same max_dist, any one of them can be chosen.
            if (dist[u] > max_dist) {
                max_dist = dist[u];
                farthest_node = u;
            }

            // Explore neighbors.
            for (int v : adj_list[u]) {
                if (dist[v] == -1) { // If neighbor 'v' has not been visited yet.
                    dist[v] = dist[u] + 1; // Distance to 'v' is one more than to 'u'.
                    q.push(v);
                }
            }
        }
        // Return the farthest node found from start_node and its distance.
        return {farthest_node, max_dist};
    }

    // Main function to solve the Party in Town problem.
    // It finds the radius of the tree.
    // Renamed from getPartyHouse to partyHouse and adjusted parameters based on judge diagnostic.
    int partyHouse(std::vector<std::vector<int>>& adj) {
        int n = adj.size(); // Derive n from the size of the adjacency list.

        // Handle edge cases for a single node or empty graph.
        // Problem constraints usually guarantee n >= 1.
        if (n == 0) return 0; // No houses, no party.
        if (n == 1) return 0; // One house, max distance is 0.

        // Build a 0-indexed adjacency list from the given 1-indexed input.
        // The input `adj[i]` contains neighbors of house `i+1`.
        // The graph is undirected, and `adj` is provided as a complete adjacency list
        // (i.e., if A is connected to B, B is in adj[A-1] and A is in adj[B-1]).
        std::vector<std::vector<int>> adj_list(n);
        for (int i = 0; i < n; ++i) { // For each house i+1 (0-indexed i)
            for (int neighbor_1_indexed : adj[i]) {
                // Convert 1-indexed neighbor to 0-indexed.
                int neighbor_0_indexed = neighbor_1_indexed - 1;
                adj_list[i].push_back(neighbor_0_indexed);
            }
        }

        // Step 1: Find one endpoint of a diameter.
        // Start BFS from an arbitrary node (e.g., node 0).
        // The farthest node from node 0 (let's call it 'u') must be an endpoint of some diameter.
        std::pair<int, int> result1 = bfs(0, n, adj_list);
        int u = result1.first; // 'u' is one endpoint of a diameter.

        // Step 2: Find the other endpoint of the diameter and its length.
        // Start BFS from 'u'. The farthest node from 'u' will be the other endpoint of a diameter.
        // The distance from 'u' to this farthest node is the length of the diameter.
        std::pair<int, int> result2 = bfs(u, n, adj_list);
        int diameter = result2.second; // This is the length of the tree's diameter.

        // Step 3: The minimum possible maximum distance (radius) is ceil(diameter / 2).
        // In integer arithmetic, ceil(D/2) can be calculated as (D + 1) / 2.
        return (diameter + 1) / 2;
    }
};