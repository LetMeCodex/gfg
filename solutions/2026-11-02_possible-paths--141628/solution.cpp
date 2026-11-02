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
    // Renamed the function from 'countPaths' to 'maximumWeight'
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