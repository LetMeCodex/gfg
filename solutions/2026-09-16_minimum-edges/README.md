# [Min Edge Reversals for Path](https://www.geeksforgeeks.org/problems/minimum-edges/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary

Given a directed graph with $N$ nodes and $M$ edges, where each edge is represented as a pair $(u, v)$ indicating a directed edge from $u$ to $v$. We are also given a source node `src` and a destination node `dst`. The goal is to find the minimum number of edge reversals required to make a path from `src` to `dst` possible. If no path can be formed even with reversals, return -1.

## Intuition & Approach

The problem asks for the minimum number of edge reversals to create a path. This hints at a shortest path problem. However, the "cost" of traversing an edge depends on whether we use it in its original direction or reverse it.

Let's consider an edge from node $u$ to node $v$.
1.  If we traverse this edge in its original direction ($u \to v$), it costs **0 reversals**.
2.  If we want to traverse this edge in the opposite direction ($v \to u$), we must **reverse** the original edge $u \to v$. This costs **1 reversal**.

This structure, where edge weights are either 0 or 1, is a perfect candidate for a **0-1 Breadth-First Search (BFS)**. A standard BFS explores layer by layer, assuming all edge weights are 1. Dijkstra's algorithm handles varying positive edge weights. However, for 0-1 weights, 0-1 BFS is more efficient.

**0-1 BFS Approach:**

1.  **Graph Representation:** We can represent the graph using an adjacency list. For each original edge $(u, v)$:
    *   Add a directed edge from $u$ to $v$ with a weight of 0 (representing no reversal needed).
    *   Add a directed edge from $v$ to $u$ with a weight of 1 (representing one reversal needed).

2.  **Distance Array:** We'll use a `dist` array to store the minimum number of reversals required to reach each node from the `src`. Initialize all distances to infinity, except for `dist[src]`, which is 0.

3.  **Deque for 0-1 BFS:** A `deque` (double-ended queue) is used to manage nodes to visit.
    *   When we encounter an edge with weight 0 (original direction), we push the destination node to the **front** of the deque. This prioritizes exploring paths that don't require reversals.
    *   When we encounter an edge with weight 1 (reversed direction), we push the destination node to the **back** of the deque. These paths are explored later as they incur a cost.

4.  **BFS Traversal:**
    *   Start by pushing `src` to the front of the deque and setting `dist[src] = 0`.
    *   While the deque is not empty:
        *   Dequeue a node `u` from the front.
        *   For each neighbor `v` of `u` with edge weight `w` (0 or 1):
            *   If `dist[u] + w < dist[v]`:
                *   Update `dist[v] = dist[u] + w`.
                *   If `w` is 0, push `v` to the front of the deque.
                *   If `w` is 1, push `v` to the back of the deque.

5.  **Result:** After the BFS completes, `dist[dst]` will contain the minimum number of edge reversals needed to reach `dst` from `src`. If `dist[dst]` is still infinity, it means `dst` is unreachable, and we return -1.

**Why 3 Attempts?**

The initial attempts likely involved:
1.  **Standard BFS/Dijkstra:** A standard BFS or Dijkstra without correctly modeling the edge reversal cost would fail. For example, a standard BFS would treat all edges as having weight 1, not distinguishing between original and reversed edges. Dijkstra might work if implemented correctly with the 0/1 weights, but 0-1 BFS is specifically optimized for this scenario.
2.  **Incorrect Graph Construction:** Misinterpreting how to represent reversed edges in the adjacency list could lead to errors. For instance, only adding the original edge or incorrectly assigning weights.
3.  **Off-by-one errors or Initialization:** Issues with 1-based vs. 0-based indexing for nodes, or incorrect initialization of the distance array, are common pitfalls. The problem statement uses 1-based indexing for nodes, which needs careful handling.

The 0-1 BFS approach elegantly handles the varying costs and ensures we find the path with the minimum number of reversals.

## Complexity Analysis

-   **Time Complexity**: $O(N + M)$
    *   Building the adjacency list takes $O(M)$ time, as we iterate through all $M$ edges and add two entries for each.
    *   The 0-1 BFS visits each node at most once. For each node, it iterates through its outgoing edges. The total number of edges in our constructed graph is $2M$ (one for each original edge and one for its reversal). Therefore, the BFS traversal takes $O(N + 2M) = O(N + M)$ time.
    *   Overall, the time complexity is dominated by the graph construction and BFS, resulting in $O(N + M)$.

-   **Space Complexity**: $O(N + M)$
    *   The adjacency list `adj` stores $2M$ edges, so it takes $O(N + M)$ space.
    *   The `dist` array takes $O(N)$ space.
    *   The deque `dq` can store up to $N$ nodes in the worst case, taking $O(N)$ space.
    *   Therefore, the total space complexity is $O(N + M)$.

## Solution Code

```cpp
#include <vector>
#include <deque>
#include <limits> // Required for std::numeric_limits

class Solution {
public:
    /**
     * @brief Finds the minimum number of edge reversals required to create a path from src to dst.
     * 
     * @param edges A list of edges, where each edge is represented as {u, v} (directed from u to v).
     * @param n The number of nodes in the graph (nodes are 1-indexed).
     * @param src The source node.
     * @param dst The destination node.
     * @return The minimum number of edge reversals, or -1 if no path is possible.
     */
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

            // If we have already found a shorter path to 'u' after it was added to the deque,
            // we can skip processing it again. This is an optimization for 0-1 BFS.
            // However, the core logic below handles this implicitly by checking `dist[u] + weight < dist[v]`.
            // For clarity and correctness, we proceed with checking neighbors.

            // Iterate over all neighbors of u.
            for (const auto& edge_info : adj[u]) {
                int v = edge_info.first;
                int weight = edge_info.second; // 0 for original, 1 for reversed

                // If a shorter path to v is found through u.
                if (dist[u] != std::numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
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
```