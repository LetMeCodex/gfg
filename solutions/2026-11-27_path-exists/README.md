# Path Exists Between Two Vertices

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to determine if a path exists between a given source vertex `u` and a destination vertex `v` in an undirected graph. The graph is represented by an adjacency list.

## Intuition & Approach

This problem is a classic graph traversal problem. To check if a path exists between two vertices, we can use either Breadth-First Search (BFS) or Depth-First Search (DFS). Both algorithms systematically explore the graph starting from a source vertex and can detect connectivity.

For this solution, I've chosen to implement **Breadth-First Search (BFS)**.

**BFS Intuition:**
BFS explores the graph layer by layer. It starts at the source vertex, visits all its immediate neighbors, then visits all their unvisited neighbors, and so on. If the destination vertex is encountered during this exploration, it means a path exists.

**Algorithm Steps (BFS):**

1.  **Initialization:**
    *   Create a `visited` array (or set) to keep track of visited vertices. Initialize all vertices as not visited.
    *   Create a queue and enqueue the starting vertex `u`.
    *   Mark the starting vertex `u` as visited.

2.  **Traversal:**
    *   While the queue is not empty:
        *   Dequeue a vertex, let's call it `current_vertex`.
        *   If `current_vertex` is the destination vertex `v`, then a path exists, and we can return `true`.
        *   For each `neighbor` of `current_vertex`:
            *   If `neighbor` has not been visited:
                *   Mark `neighbor` as visited.
                *   Enqueue `neighbor`.

3.  **No Path Found:**
    *   If the queue becomes empty and the destination vertex `v` has not been reached, it means there is no path from `u` to `v`. Return `false`.

**Why BFS?**
BFS is generally preferred for finding the shortest path in an unweighted graph, but for simply checking connectivity, both BFS and DFS work equally well. BFS's layer-by-layer exploration can sometimes be more intuitive for understanding reachability.

**Second Attempt Rationale:**
The first attempt likely involved a minor mistake in handling the graph representation or the visited array logic. Common pitfalls include:
*   Incorrectly initializing the `visited` array.
*   Not marking a vertex as visited *before* enqueuing it, leading to redundant processing.
*   Off-by-one errors in vertex indexing if the graph is 0-indexed or 1-indexed.
*   Forgetting to handle the case where `u` and `v` are the same vertex (though the BFS naturally handles this).

The provided solution correctly implements the BFS algorithm, ensuring that each vertex is visited and processed only once, and the destination is checked upon dequeueing.

## Complexity Analysis

-   **Time Complexity**: $O(V + E)$
    *   Where $V$ is the number of vertices and $E$ is the number of edges in the graph.
    *   In the worst case, BFS visits every vertex and traverses every edge once. Each vertex is enqueued and dequeued at most once. For each dequeued vertex, we iterate through its neighbors. The sum of the degrees of all vertices is $2E$ for an undirected graph.

-   **Space Complexity**: $O(V)$
    *   The `visited` vector takes $O(V)$ space.
    *   The queue can store up to $O(V)$ vertices in the worst case (e.g., a star graph where the center is the source).

## Solution Code

```cpp
#include <vector>
#include <queue>

class Solution {
public:
    /**
     * @brief Checks if a path exists between two vertices in an undirected graph using BFS.
     *
     * @param adj The adjacency list representation of the graph.
     *            adj[i] contains a list of vertices adjacent to vertex i.
     * @param u The starting vertex.
     * @param v The destination vertex.
     * @return true if a path exists from u to v, false otherwise.
     */
    bool checkPath(std::vector<std::vector<int>>& adj, int u, int v) {
        // Get the total number of vertices in the graph.
        int V = adj.size();

        // Create a boolean vector to keep track of visited vertices.
        // Initialize all vertices as not visited.
        std::vector<bool> visited(V, false);

        // Create a queue for BFS.
        std::queue<int> q;

        // Start BFS from the source vertex 'u'.
        q.push(u);
        visited[u] = true; // Mark the source vertex as visited.

        // Perform BFS traversal.
        while (!q.empty()) {
            // Dequeue a vertex from the queue.
            int current_vertex = q.front();
            q.pop();

            // If the dequeued vertex is the destination vertex 'v', a path exists.
            if (current_vertex == v) {
                return true;
            }

            // Explore all adjacent vertices of the current_vertex.
            for (int neighbor : adj[current_vertex]) {
                // If the neighbor has not been visited yet:
                if (!visited[neighbor]) {
                    // Mark the neighbor as visited.
                    visited[neighbor] = true;
                    // Enqueue the neighbor to explore it later.
                    q.push(neighbor);
                }
            }
        }

        // If the queue becomes empty and 'v' was not reached, no path exists.
        return false;
    }
};
```