# Count Tree Paths With Limited Max Edge Weight

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find, for each given query, the total number of paths between any two distinct nodes in a graph such that all edges on that path have a weight less than or equal to a specified maximum weight for that query. We are given a graph with $N$ nodes and a list of edges, and a list of queries, where each query specifies a maximum allowed edge weight.

## Intuition & Approach

The core idea is to efficiently count paths whose edges are all below a certain weight threshold. A brute-force approach of checking every path for every query would be too slow. The "Hard" difficulty and the nature of path counting with edge constraints strongly suggest a Disjoint Set Union (DSU) data structure combined with a sorting-based approach, similar to Kruskal's algorithm for Minimum Spanning Tree.

Let's break down the intuition:

1.  **Focus on Edge Weights:** The constraint is on the maximum edge weight. This means if we consider edges in increasing order of their weights, we can incrementally build up connected components.

2.  **DSU for Connected Components:** When we consider edges up to a certain weight $W$, all nodes connected by these edges will form connected components. If two nodes are in the same connected component, there exists at least one path between them using only edges with weights $\le W$.

3.  **Counting Paths within Components:** If a connected component has $k$ nodes, how many paths can be formed between distinct pairs of nodes *within this component* using only edges $\le W$?
    *   Any pair of distinct nodes $(u, v)$ within this component will have at least one path between them.
    *   The total number of distinct pairs of nodes in a component of size $k$ is $\binom{k}{2} = \frac{k(k-1)}{2}$.
    *   However, the problem asks for the number of *paths*, not just pairs. A key insight here is that when we merge two components of sizes $s_i$ and $s_j$ using an edge of weight $\le W$, we form $s_i \times s_j$ *new* paths. These new paths are formed by taking any node from the first component and any node from the second component and connecting them via the edge that caused the merge (and potentially other edges within their respective components).
    *   Therefore, if we maintain the total number of paths formed so far, and when we merge two components of size $s_i$ and $s_j$, we add $s_i \times s_j$ to our total count.

4.  **Processing Queries Efficiently:**
    *   We have multiple queries, each with a different maximum allowed edge weight.
    *   If we sort the edges by weight in ascending order, and also sort the queries by their maximum allowed weight in ascending order, we can process them together.
    *   We use two pointers: one for the sorted edges and one for the sorted queries.
    *   As we iterate through the sorted queries, for a query with maximum weight $W_{query}$:
        *   We add all edges whose weight is less than or equal to $W_{query}$ into our DSU structure.
        *   The DSU's `unite` operation will handle merging components and updating the total path count.
        *   Once all edges $\le W_{query}$ have been processed, the current total path count in our DSU structure is the answer for this query.

**Algorithm Steps:**

1.  **Represent Edges and Queries:**
    *   Create a structure `Edge` to store `u`, `v`, and `w`.
    *   Create a structure `Query` to store `val` (maximum weight) and `original_idx` (to store the answer in the correct order).

2.  **Sort Edges:** Sort the `edges` vector in ascending order based on their weights (`w`).

3.  **Sort Queries:** Sort the `queries` vector in ascending order based on their maximum allowed weight (`val`).

4.  **Initialize DSU:** Create a DSU instance for $N$ nodes. Initialize `parent` array and `sz` (size of component) array. The `sz` array is crucial for calculating new paths upon merging.

5.  **Initialize Path Count:** Initialize a `long long` variable `current_total_paths` to 0. This will store the cumulative count of valid paths.

6.  **Two-Pointer Iteration:**
    *   Use a pointer `edge_ptr` for the sorted `edges` array, starting at 0.
    *   Iterate through the sorted `queries`. For each `query`:
        *   While `edge_ptr` is within bounds and `edges[edge_ptr].w <= query.val`:
            *   Call `dsu.unite(edges[edge_ptr].u, edges[edge_ptr].v, current_total_paths)`. The `unite` function will merge the components of `u` and `v` if they are different, and crucially, it will update `current_total_paths` by adding `size(component_u) * size(component_v)`.
            *   Increment `edge_ptr`.
        *   After the inner `while` loop finishes, all edges with weight $\le query.val$ have been considered. The `current_total_paths` now holds the correct count for this query.
        *   Store `current_total_paths` in the `ans` array at `query.original_idx`.

7.  **Return Result:** Return the `ans` array.

**DSU `unite` function modification:**
The standard DSU `unite` function needs to be augmented. When merging two sets with roots `root_i` and `root_j` of sizes `sz[root_i]` and `sz[root_j]` respectively, the number of new paths formed is `sz[root_i] * sz[root_j]`. This value should be added to the global `current_total_paths`.

## Complexity Analysis

-   **Time Complexity**:
    *   Sorting edges: $O(E \log E)$, where $E$ is the number of edges.
    *   Sorting queries: $O(Q \log Q)$, where $Q$ is the number of queries.
    *   DSU operations: With path compression and union by size/rank, each `find` and `unite` operation takes nearly constant time on average, amortized $O(\alpha(N))$, where $\alpha$ is the inverse Ackermann function, which is very slow-growing and practically constant.
    *   The two-pointer iteration: Each edge is processed by the `unite` operation at most once. Each query is processed once.
    *   Total time for DSU operations and iteration: $O(E \alpha(N) + Q)$.
    *   Overall Time Complexity: $O(E \log E + Q \log Q)$.

-   **Space Complexity**:
    *   Storing edges: $O(E)$.
    *   Storing queries: $O(Q)$.
    *   DSU `parent` and `sz` arrays: $O(N)$.
    *   Result array: $O(Q)$.
    *   Overall Space Complexity: $O(N + E + Q)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // For std::sort, std::swap
#include <numeric>   // For std::iota

// DSU (Disjoint Set Union) data structure
struct DSU {
    std::vector<int> parent;
    std::vector<long long> sz; // Stores size of the component. Using long long for safety.

    DSU(int n) {
        parent.resize(n + 1);
        std::iota(parent.begin(), parent.end(), 0); // Initialize parent[i] = i for i from 0 to n
        sz.assign(n + 1, 1); // Each node initially forms a component of size 1
    }

    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]); // Path compression for efficiency
    }

    // Unites the sets containing i and j.
    // Updates current_total_paths by the number of new paths formed due to the merge.
    // Returns true if a merge happened, false if i and j were already in the same set.
    bool unite(int i, int j, long long& current_total_paths) {
        int root_i = find(i);
        int root_j = find(j);

        if (root_i != root_j) {
            // Union by size: attach the smaller tree under the root of the larger tree
            if (sz[root_i] < sz[root_j])
                std::swap(root_i, root_j); // Ensure root_i is the root of the larger or equal-sized component
            
            // When merging two components of sizes s_i and s_j,
            // the number of new paths formed between nodes from different components is s_i * s_j.
            // This is because any node from the first component can form a path with any node from the second component.
            current_total_paths += sz[root_i] * sz[root_j];

            parent[root_j] = root_i; // Attach root_j's component to root_i
            sz[root_i] += sz[root_j]; // Update the size of the new merged component
            return true;
        }
        return false; // Nodes i and j were already in the same set
    }
};

// Structure to represent an edge
struct Edge {
    int u, v, w;
};

// Structure to represent a query, storing its value and original index
struct Query {
    int val;
    int original_idx;
};

class Solution {
public:
    // Renamed the function from 'countPaths' to 'maximumWeight' as per the problem statement
    std::vector<int> maximumWeight(int n, std::vector<std::vector<int>>& edges_input, std::vector<int>& queries_input) {
        // 1. Prepare and sort edges
        std::vector<Edge> edges;
        for (const auto& edge_arr : edges_input) {
            edges.push_back({edge_arr[0], edge_arr[1], edge_arr[2]});
        }
        // Sort edges by weight in ascending order. This is crucial for the Kruskal-like approach.
        std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.w < b.w;
        });

        // 2. Prepare and sort queries
        std::vector<Query> queries;
        for (int i = 0; i < queries_input.size(); ++i) {
            queries.push_back({queries_input[i], i});
        }
        // Sort queries by their maximum allowed edge weight in ascending order.
        // This allows us to process edges incrementally.
        std::sort(queries.begin(), queries.end(), [](const Query& a, const Query& b) {
            return a.val < b.val;
        });

        // 3. Initialize DSU and result array
        DSU dsu(n);
        std::vector<int> ans(queries_input.size());
        // current_total_paths stores the cumulative count of valid paths.
        // Using long long for current_total_paths to prevent potential overflow,
        // although for N=10^4, the max total paths (N*(N-1)/2 ~ 5*10^7) fits in int.
        // The intermediate product sz[root_i] * sz[root_j] could potentially exceed INT_MAX if N was larger,
        // but for N=10^4, max product is 10^4 * 10^4 = 10^8, which also fits in int.
        // Still, long long is a safer general practice.
        long long current_total_paths = 0; 

        // 4. Process queries and edges using two pointers (Kruskal-like approach)
        int edge_ptr = 0; // Pointer for the sorted edges list
        for (const auto& q : queries) {
            // Add all edges whose weight is less than or equal to the current query's max allowed weight (q.val).
            // Since both edges and queries are sorted by weight, we only iterate edge_ptr forward.
            while (edge_ptr < edges.size() && edges[edge_ptr].w <= q.val) {
                // Attempt to unite the components connected by the current edge.
                // The unite function updates current_total_paths if a merge occurs.
                dsu.unite(edges[edge_ptr].u, edges[edge_ptr].v, current_total_paths);
                edge_ptr++;
            }
            // After processing all relevant edges for this query, store the current total number of paths.
            // Cast to int as the return type is vector<int>.
            ans[q.original_idx] = static_cast<int>(current_total_paths);
        }

        return ans;
    }
};
```