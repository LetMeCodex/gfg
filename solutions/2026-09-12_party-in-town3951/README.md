# [Party in Town](https://www.geeksforgreeks.org/problems/party-in-town3951/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find a "party house" in a town, which is represented as a tree (a connected, undirected graph with no cycles). We are given `N` houses and `N-1` roads connecting them. All roads have a length of 1 unit. A party house is defined as a house such that the maximum distance from it to any other house in the town is minimized. Our task is to return this minimum possible maximum distance.

In graph theory terms, this problem asks us to find the **radius** of a tree. The radius of a graph is the minimum eccentricity of any vertex, where the eccentricity of a vertex `v` is the maximum distance from `v` to any other vertex in the graph. The set of all vertices whose eccentricity equals the radius is called the **center** of the tree.

## Intuition & Approach

The core of this problem lies in understanding the properties of trees, specifically their diameter and radius.

1.  **Diameter of a Tree**: The diameter of a tree is the longest path between any two nodes in the tree.
2.  **Radius of a Tree**: The radius of a tree is the minimum possible value of the maximum distance from a node to any other node in the tree. This minimum value is achieved at the tree's center(s).

A well-known algorithm to find the diameter (and subsequently the radius) of a tree involves two Breadth-First Search (BFS) traversals:

### Step-by-step Approach:

1.  **First BFS to find one endpoint of a diameter:**
    *   Start a BFS from an arbitrary node (e.g., node 0).
    *   Perform a standard BFS traversal, keeping track of the distance from the starting node to every other reachable node.
    *   Identify the node `u` that is farthest from the initial starting node. A crucial property of trees states that any node farthest from an arbitrary starting node must be an endpoint of *some* diameter of the tree.

2.  **Second BFS to find the diameter length:**
    *   Now, start a second BFS from node `u` (the farthest node found in Step 1).
    *   Again, perform a standard BFS, calculating distances from `u` to all other nodes.
    *   Identify the node `v` that is farthest from `u`. The path between `u` and `v` represents a diameter of the tree, and the distance `dist(u, v)` is the length of this diameter.

3.  **Calculate the Radius:**
    *   The radius of a tree is directly related to its diameter. Specifically, the radius is `ceil(diameter / 2)`.
    *   In integer arithmetic, `ceil(D / 2)` can be calculated as `(D + 1) / 2`. This is because if `D` is even, `D/2` is correct. If `D` is odd, `(D+1)/2` correctly rounds up.

### Why this works:

The center(s) of a tree (the node(s) that minimize the maximum distance to any other node) always lie on every diameter of the tree. By finding a diameter, we effectively locate the "longest stretch" of the tree. The center will be roughly in the middle of this longest stretch, ensuring that the maximum distance to any leaf node (which are often at the ends of diameters) is minimized.

### Implementation Details:

*   **Adjacency List Conversion**: The problem input `adj` is a `vector<vector<int>>` where `adj[i]` contains neighbors of house `i+1`. This implies 1-indexed houses. My solution first converts this into a 0-indexed adjacency list (`adj_list`) for easier internal processing.
*   **`bfs` Helper Function**: A helper function `bfs(start_node, n, adj_list)` is used to perform a BFS traversal. It returns a `pair<int, int>`: `{farthest_node_index, max_distance_from_start_node}`. This function efficiently finds the farthest node and its distance from a given starting point.
*   **Edge Cases**: Handles `n=0` (no houses) and `n=1` (one house) gracefully, returning 0 as the maximum distance is 0.

## Complexity Analysis

Let `N` be the number of houses (nodes) and `M` be the number of roads (edges). For a tree, `M = N - 1`.

*   **Time Complexity**:
    *   **Building `adj_list`**: We iterate through the input `adj` list. In the worst case, this involves iterating through all `N` houses and their respective neighbors. The total number of neighbor entries across all houses is `2 * M` (since each edge is represented twice). Thus, this step takes `O(N + M)` time. For a tree, `M = N - 1`, so it's `O(N)`.
    *   **`bfs` function**: A standard BFS traversal visits each node and each edge exactly once. This takes `O(N + M)` time. Since `M = N - 1` for a tree, each BFS call takes `O(N)` time.
    *   **Total BFS calls**: We perform two BFS calls.
    *   Therefore, the total time complexity is `O(N) + O(N) + O(N) = O(N)`.

*   **Space Complexity**:
    *   **`adj_list`**: Stores `N` vectors and a total of `2 * M` integers for edges. This requires `O(N + M)` space. For a tree, it's `O(N)`.
    *   **`dist` vector in `bfs`**: Stores `N` integers to keep track of distances. This requires `O(N)` space.
    *   **`queue` in `bfs`**: In the worst case (e.g., a star graph), the queue can hold up to `N-1` nodes. This requires `O(N)` space.
    *   Therefore, the total space complexity is `O(N)`.

## Solution Code

```cpp
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
```