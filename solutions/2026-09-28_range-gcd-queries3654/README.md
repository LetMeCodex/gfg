# GeeksforGeeks POTD: Range GCD Queries

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to implement a data structure that can efficiently handle two types of operations on an array of integers:
1.  **Range GCD Query**: Given a range `[L, R]`, find the Greatest Common Divisor (GCD) of all elements in the array from index `L` to `R` (inclusive).
2.  **Point Update**: Given an index `idx` and a new value `val`, update the element at `arr[idx]` to `val`.

We are given an initial array and a list of queries. We need to return the results of all the range GCD queries.

## Intuition & Approach

This problem is a classic application of **Segment Trees**. A segment tree is a tree data structure used for storing information about intervals or segments. It allows querying and updating operations on an array in logarithmic time.

For this specific problem, each node in the segment tree will store the GCD of the elements in the range it represents.

**1. Building the Segment Tree:**
   - The segment tree will be a binary tree.
   - The root will represent the entire array `[0, n-1]`.
   - Each internal node will represent a range `[start, end]`. Its left child will represent `[start, mid]` and its right child will represent `[mid+1, end]`, where `mid = start + (end - start) / 2`.
   - Leaf nodes will represent individual elements of the array.
   - The value stored in an internal node will be the GCD of the values stored in its children.
   - The `build` function recursively constructs the tree. For a leaf node, it stores the array element. For an internal node, it recursively builds its children and then computes its own value as `gcd(leftChildValue, rightChildValue)`.

**2. Range GCD Query:**
   - The `query` function takes the current node, its range `[start, end]`, and the query range `[l, r]`.
   - **Case 1: No Overlap**: If the current node's range `[start, end]` is completely outside the query range `[l, r]` (i.e., `r < start` or `end < l`), we return `0`. This is because `gcd(x, 0) = x`, so `0` acts as an identity element for GCD operations when combining results.
   - **Case 2: Complete Overlap**: If the current node's range `[start, end]` is completely inside the query range `[l, r]` (i.e., `l <= start` and `end <= r`), we return the value stored in the current node.
   - **Case 3: Partial Overlap**: If there's a partial overlap, we recursively query the left and right children for their respective overlapping portions of the query range. The final result is the GCD of the results from the left and right children.

**3. Point Update:**
   - The `update` function takes the current node, its range `[start, end]`, the index to update `idx`, and the new value `val`.
   - **Base Case**: If `start == end` (we've reached the leaf node corresponding to `idx`), update the value in the tree node to `val`.
   - **Recursive Step**: If `idx` falls within the left child's range, recurse on the left child. Otherwise, recurse on the right child.
   - After the recursive call returns, update the current node's value by taking the GCD of its (potentially updated) children's values. This propagates the change up the tree.

**GCD Function:**
   - The problem statement implies that a `gcd` function is available. If not, a standard Euclidean algorithm implementation would be needed. The provided solution includes a static `gcd` helper function for completeness, assuming the judge might not provide a global one or if `std::gcd` (C++17) is not guaranteed.

**Implementation Details:**
   - The segment tree is typically implemented using an array. For an array of size `N`, the segment tree array needs a size of `4*N` to accommodate all nodes.
   - Node indexing: If a node is at index `i`, its left child is at `2*i` and its right child is at `2*i + 1`. The root is usually at index `1`.
   - The `processQueries` function orchestrates the building of the tree and then iterates through the queries, calling the appropriate `query` or `update` methods.

## Complexity Analysis

-   **Time Complexity**:
    -   **Building the Segment Tree**: Each element of the array is processed once to build the leaf nodes, and each internal node is computed once. There are `O(N)` nodes in the segment tree. Thus, building takes $O(N \log N)$ time if the GCD operation takes $O(\log(\max(a, b)))$ time, or $O(N)$ if GCD is considered $O(1)$ for practical purposes in competitive programming.
    -   **Range GCD Query**: A query traverses a path from the root to some leaf nodes, visiting at most `O(log N)` nodes at each level. Since there are `O(log N)` levels, a query takes $O(\log N)$ time.
    -   **Point Update**: An update also traverses a path from the root to a leaf node and back up, updating `O(log N)` nodes. Thus, an update takes $O(\log N)$ time.
    -   **Total for `processQueries`**: If there are `Q` queries, the total time complexity is $O(N \log N + Q \log N)$.

-   **Space Complexity**:
    -   The segment tree requires an array of size `4*N` to store its nodes. Therefore, the space complexity is $O(N)$.

## Solution Code

```cpp
#include <vector>
#include <numeric> // For std::gcd in C++17, but we'll use a custom one for broader compatibility.
#include <algorithm> // For std::swap

class Solution {
private:
    std::vector<int> tree;
    int n;

    // Custom GCD function using Euclidean algorithm.
    // This is used because std::gcd is C++17, and judge environments might not support it.
    // If the judge guarantees std::gcd or a global gcd function, this can be removed.
    static int gcd(int a, int b) {
        // Ensure non-negative inputs for GCD, though problem constraints usually handle this.
        // The identity element for GCD is 0, so gcd(x, 0) = x.
        // If a or b is 0, the other value is the GCD.
        if (a == 0) return b;
        if (b == 0) return a;

        while (b) {
            a %= b;
            std::swap(a, b);
        }
        return a;
    }

    // Builds the segment tree.
    // node: current node index in the 'tree' vector.
    // start, end: the range represented by the current node.
    // arr: the input array.
    void build(int node, int start, int end, const std::vector<int>& arr) {
        if (start == end) {
            // Leaf node: store the array element.
            tree[node] = arr[start];
        } else {
            int mid = start + (end - start) / 2; // Calculate mid safely to prevent overflow.
            // Recursively build left and right children.
            build(2 * node, start, mid, arr);
            build(2 * node + 1, mid + 1, end, arr);
            // Internal node: store the GCD of its children.
            tree[node] = gcd(tree[2 * node], tree[2 * node + 1]);
        }
    }

    // Updates the value at a specific index in the array and propagates the change up the tree.
    // node: current node index.
    // start, end: range represented by the current node.
    // idx: the index to update.
    // val: the new value.
    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            // Leaf node found: update its value.
            tree[node] = val;
        } else {
            int mid = start + (end - start) / 2;
            if (start <= idx && idx <= mid) {
                // Index is in the left child's range.
                update(2 * node, start, mid, idx, val);
            } else {
                // Index is in the right child's range.
                update(2 * node + 1, mid + 1, end, idx, val);
            }
            // Update the current node's GCD based on its children's updated values.
            tree[node] = gcd(tree[2 * node], tree[2 * node + 1]);
        }
    }

    // Queries the GCD of elements in a given range [l, r].
    // node: current node index.
    // start, end: range represented by the current node.
    // l, r: the query range.
    int query(int node, int start, int end, int l, int r) {
        // Case 1: Current segment is completely outside the query range.
        // Return 0, which is the identity element for GCD (gcd(x, 0) = x).
        if (r < start || end < l) {
            return 0;
        }
        // Case 2: Current segment is completely inside the query range.
        // Return the pre-calculated GCD for this segment.
        if (l <= start && end <= r) {
            return tree[node];
        }
        // Case 3: Current segment partially overlaps with the query range.
        // Recurse on children and combine their results.
        int mid = start + (end - start) / 2;
        int p1 = query(2 * node, start, mid, l, r);
        int p2 = query(2 * node + 1, mid + 1, end, l, r);
        return gcd(p1, p2);
    }

public:
    // Processes all the given queries.
    // arr: the initial array.
    // queries: a list of queries, where each query is a vector of integers.
    //          query[0] == 0 for range GCD query (query[1]=l, query[2]=r).
    //          query[0] == 1 for point update (query[1]=index, query[2]=value).
    std::vector<int> processQueries(std::vector<int>& arr, std::vector<std::vector<int>>& queries) {
        n = arr.size();
        // The segment tree array needs approximately 4*N space.
        tree.resize(4 * n);

        // Build the segment tree from the initial array.
        build(1, 0, n - 1, arr);

        std::vector<int> results;
        for (const auto& query_item : queries) {
            if (query_item[0] == 0) { // Type 0 query: Range GCD
                int l = query_item[1];
                int r = query_item[2];
                results.push_back(query(1, 0, n - 1, l, r));
            } else { // Type 1 query: Update
                int index = query_item[1];
                int value = query_item[2];
                update(1, 0, n - 1, index, value);
            }
        }
        return results;
    }
};
```