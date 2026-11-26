# [Count Edges in Graph](https://www.geeksforgreeks.org/problems/count-number-of-edges-in-an-undirected-graph/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to count the total number of edges in an undirected graph. The graph is provided in the form of an adjacency list, where `adj[i]` contains a list of all vertices directly connected to vertex `i`. We need to return the total count of unique edges in this graph.

## Intuition & Approach

The core idea behind solving this problem efficiently for an undirected graph represented by an adjacency list lies in a fundamental graph theory concept known as the **Handshaking Lemma**.

**Handshaking Lemma**: In any undirected graph, the sum of the degrees of all vertices is equal to twice the number of edges.
Mathematically: $\sum_{v \in V} \text{degree}(v) = 2 \times |E|$

Here's how we apply this:
1.  **Degree of a Vertex**: For a graph represented by an adjacency list `adj`, the degree of any vertex `i` is simply the number of neighbors it has. In our `std::vector<std::vector<int>> adj` structure, `adj[i].size()` directly gives us the degree of vertex `i`.
2.  **Sum of Degrees**: We can iterate through each vertex in the graph (from `0` to `N-1`, where `N` is the number of vertices). For each vertex, we calculate its degree using `adj[i].size()` and add it to a running total. This total will be the `sum_of_degrees`.
3.  **Calculate Edges**: Once we have the `sum_of_degrees`, according to the Handshaking Lemma, the total number of edges will be `sum_of_degrees / 2`. We divide by 2 because each edge connects two vertices, and thus contributes to the degree count of both its endpoints. When we sum all degrees, each edge is counted exactly twice.

**Example**:
Consider a graph with vertices 0, 1, 2 and edges (0,1), (1,2).
Adjacency List:
`adj[0] = {1}` (degree 1)
`adj[1] = {0, 2}` (degree 2)
`adj[2] = {1}` (degree 1)

Sum of degrees = `1 + 2 + 1 = 4`
Number of edges = `4 / 2 = 2`. This matches the actual edges (0,1) and (1,2).

This approach is straightforward and leverages a direct property of undirected graphs.

## Complexity Analysis

Let `N` be the number of vertices and `E` be the number of edges in the graph.

-   **Time Complexity**: $O(N + E)$
    -   We iterate through each of the `N` entries in the `adj` vector (the outer loop `for (const auto& neighbors : adj)`).
    -   For each entry `neighbors`, we call `neighbors.size()`, which is an $O(1)$ operation for `std::vector`.
    -   The sum `total_degree` accumulates the sizes of all adjacency lists. The sum of `neighbors.size()` for all `neighbors` is exactly the sum of degrees of all vertices, which is $2E$.
    -   Therefore, the total time spent iterating through the `adj` vector and summing up the sizes is proportional to `N` (for iterating through the outer vector) plus `2E` (for effectively "visiting" each entry in all adjacency lists).
    -   Hence, the time complexity is $O(N + E)$.

-   **Space Complexity**: $O(1)$
    -   We use a single integer variable `total_degree` to store the sum of degrees.
    -   No additional data structures are allocated that depend on the input size.
    -   The input adjacency list `adj` is not counted as extra space.
    -   Thus, the space complexity is constant.

## Solution Code

```cpp
#include <vector>
#include <numeric> // Not strictly needed for this solution, but often useful for vector operations

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
        
        // Iterate through each vertex's adjacency list
        // 'neighbors' represents adj[i] for the current vertex i
        for (const auto& neighbors : adj) {
            // The size of the adjacency list for a vertex is its degree
            total_degree += neighbors.size();
        }
        
        // In an undirected graph, the sum of degrees of all vertices is twice the number of edges.
        // Therefore, the number of edges is half of the total sum of degrees.
        return total_degree / 2;
    }
};

```