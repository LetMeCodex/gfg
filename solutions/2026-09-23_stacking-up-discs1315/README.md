# [Maximum Height Disc Stack](https://www.geeksforgeeks.org/problems/stacking-up-discs1315/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 3 attempt(s)

## Problem Summary

The problem asks us to find the maximum possible height of a stack of discs that can be formed. We are given `N` discs, each with a radius `r` and a height `h`. A disc `i` can be placed on top of disc `j` if and only if `r_i < r_j` and `h_i < h_j`. We want to maximize the sum of heights of the discs in the stack.

## Intuition & Approach

This problem can be modeled as a variation of the Longest Increasing Subsequence (LIS) problem, but in two dimensions. The conditions `r_i < r_j` and `h_i < h_j` for placing disc `i` on top of disc `j` suggest a dependency on both radius and height.

A naive approach would be to try all possible combinations of discs, which would be exponential. We need a more efficient dynamic programming or greedy approach.

The key insight comes from how we can process the discs. If we sort the discs, we can potentially build up the solution. Let's consider the conditions for stacking: `r_i < r_j` and `h_i < h_j`.

If we sort the discs primarily by radius in ascending order, and secondarily by height in descending order (for discs with the same radius), we can process them. The secondary sort by height in descending order is crucial. If two discs have the same radius, say `r_a = r_b`, and we sort them such that `h_a > h_b`, then when we consider disc `b`, disc `a` will have already been processed. If we were to place disc `b` on top of some other disc `j`, the condition `r_b < r_j` would still hold. However, if we were to place disc `a` on top of `j`, the condition `r_a < r_j` would also hold. The secondary sort ensures that if `r_a = r_b`, we don't accidentally allow `a` to be placed on `b` or vice-versa if `h_a < h_b` (which would violate the `h_i < h_j` condition). By sorting `h` in descending order for equal `r`, we ensure that if `r_a = r_b` and `h_a > h_b`, then `a` is processed before `b`. When we consider `b`, the query for a stack below it will look for discs with `h_k < h_b`. Since `h_a > h_b`, disc `a` will not be considered as a base for `b`. This effectively enforces the strict inequality `r_i < r_j`.

After sorting, we can iterate through the discs. For each disc `i` with radius `r_i` and height `h_i`, we want to find the maximum height of a stack that can be placed *below* it. This means finding a disc `j` such that `r_i > r_j` and `h_i > h_j`. If we have already processed discs in increasing order of radius, the `r_i > r_j` condition is implicitly handled by the processing order. The challenge is efficiently finding the maximum height of a stack ending with a disc `j` where `h_j < h_i`.

This subproblem – finding the maximum stack height ending with a disc whose height is less than a certain value – can be efficiently solved using a **Segment Tree**. The segment tree will operate on the *heights* of the discs.

Here's the detailed approach:

1.  **Data Structure**: Represent each disc as a pair `(radius, height)`.
2.  **Sorting**: Sort the discs. The primary sorting key is the radius in ascending order. The secondary sorting key is the height in descending order. This ensures that when we consider a disc, any disc with a smaller radius has already been processed. For discs with the same radius, the one with greater height is processed first, preventing it from being placed on a disc with the same radius but smaller height.
3.  **Segment Tree**: We'll use a segment tree to store the maximum stack height achievable ending with a disc of a particular height. The segment tree will cover the range of possible heights (from 1 to `MAX_H_VAL`, which is 1000 based on constraints). Each leaf node `h` in the segment tree will store the maximum height of a stack whose top-most disc has height `h`. Internal nodes will store the maximum value in their range.
4.  **Iteration and DP**: Iterate through the sorted discs. For each disc `(r_i, h_i)`:
    *   **Query**: Query the segment tree for the maximum stack height achievable ending with a disc whose height is strictly less than `h_i`. This means querying the range `[1, h_i - 1]` in the segment tree. Let this maximum height be `prev_max_stack_height`.
    *   **Calculate Current Stack Height**: The total height of a stack ending with disc `i` as the top-most disc would be `h_i + prev_max_stack_height`.
    *   **Update Segment Tree**: We need to update the segment tree to reflect this new maximum stack height. The update should be at index `h_i` in the segment tree, with the value `h_i + prev_max_stack_height`. However, there's a subtlety: if multiple discs have the same radius, we must process all of them before updating the segment tree. This is because a disc `i` can only be placed on a disc `j` if `r_i < r_j`. If `r_i = r_k`, then disc `i` cannot be placed on disc `k` (and vice-versa). By delaying updates for discs with the same radius, we ensure that when we query for `prev_max_stack_height` for a disc `(r, h)`, we are only considering stacks built from discs with radii strictly less than `r`.
    *   **Handling Same Radii**: To manage this delayed update, we can use a temporary list to store the `(height, total_stack_height)` pairs for discs with the current radius. When the radius changes (or we reach the end of the discs), we then iterate through this temporary list and update the segment tree for each entry.
    *   **Track Overall Maximum**: Keep track of the maximum `current_stack_height` found across all discs.

**Example Walkthrough of Delayed Update:**

Suppose we have discs: `(5, 10), (5, 8), (7, 12)`.
Sorted: `(5, 10), (5, 8), (7, 12)` (radius ascending, height descending for same radius).

1.  **Process (5, 10)**:
    *   Query `[1, 9]` in segment tree. Assume it returns 0.
    *   `current_stack_height = 10 + 0 = 10`.
    *   Add `(10, 10)` to `updates_for_current_r_group`.
    *   `max_total_height = 10`.
    *   Radius is 5. Next disc has radius 5. Continue.

2.  **Process (5, 8)**:
    *   Query `[1, 7]` in segment tree. Assume it returns 0.
    *   `current_stack_height = 8 + 0 = 8`.
    *   Add `(8, 8)` to `updates_for_current_r_group`.
    *   `max_total_height = max(10, 8) = 10`.
    *   Radius is 5. Next disc has radius 7. Radius changes.
    *   **Apply updates for radius 5**:
        *   Update segment tree at index 10 with value 10.
        *   Update segment tree at index 8 with value 8.
    *   Clear `updates_for_current_r_group`.

3.  **Process (7, 12)**:
    *   Query `[1, 11]` in segment tree. This query will now consider stacks ending with height 8 (max height 8) and height 10 (max height 10). The maximum in `[1, 11]` will be 10.
    *   `prev_max_stack_height = 10`.
    *   `current_stack_height = 12 + 10 = 22`.
    *   Add `(12, 22)` to `updates_for_current_r_group`.
    *   `max_total_height = max(10, 22) = 22`.
    *   This is the last disc. Radius changes (implicitly).
    *   **Apply updates for radius 7**:
        *   Update segment tree at index 12 with value 22.
    *   Clear `updates_for_current_r_group`.

Final `max_total_height = 22`.

The segment tree implementation uses point updates (to set the maximum height for a specific top disc height) and range maximum queries (to find the best stack to place a new disc on). The maximum height value is 1000, so the segment tree will operate on this range.

## Complexity Analysis

-   **Time Complexity**:
    *   Sorting the discs: $O(N \log N)$, where $N$ is the number of discs.
    *   Iterating through the discs: $N$ iterations.
    *   Inside the loop:
        *   Segment tree query: $O(\log H_{max})$, where $H_{max}$ is the maximum possible height value (1000).
        *   Segment tree update: $O(\log H_{max})$.
        *   Applying updates for a group of same-radius discs: In the worst case, all discs could have the same radius, leading to $N$ updates. However, each disc is updated in the segment tree at most once across all iterations. So, the total time for all updates across all groups is $O(N \log H_{max})$.
    *   Therefore, the dominant factor is sorting and the segment tree operations.
    *   Total Time Complexity: $O(N \log N + N \log H_{max})$. Since $H_{max}$ is a constant (1000), this simplifies to $O(N \log N)$.

-   **Space Complexity**:
    *   Storing the discs: $O(N)$.
    *   Segment tree: The segment tree requires an array of size proportional to the range of heights. For a range up to $H_{max}$, the size is approximately $4 \times H_{max}$. Since $H_{max}$ is a constant, this is $O(1)$ with respect to $N$.
    *   Temporary list for updates: In the worst case, all discs could have the same radius, so this list could store up to $N$ elements. Thus, $O(N)$.
    *   Total Space Complexity: $O(N)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm>
#include <utility> // For std::pair

// Maximum possible height value for a disc.
// Used for segment tree array size and range.
// Constraints: 1 <= r[i], h[i] <= 10^3
const int MAX_H_VAL = 1000;

// Global segment tree vector.
// It's declared globally to be easily accessible by helper functions
// and reset for each test case using tree.assign().
std::vector<int> tree;

// Segment Tree: Point Update (max value)
// node: current node index in the tree array
// start, end: range covered by the current node
// idx: the specific height index to update
// val: the new maximum stack height found for a top disc of height 'idx'
void update_segment_tree(int node, int start, int end, int idx, int val) {
    if (start == end) {
        // Leaf node: update the value if 'val' is greater than current
        tree[node] = std::max(tree[node], val);
    } else {
        int mid = start + (end - start) / 2; // Use safer mid calculation
        if (start <= idx && idx <= mid) {
            // idx is in the left child's range
            update_segment_tree(2 * node, start, mid, idx, val);
        } else {
            // idx is in the right child's range
            update_segment_tree(2 * node + 1, mid + 1, end, idx, val);
        }
        // Internal node: update its value based on children's maximums
        tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
    }
}

// Segment Tree: Range Maximum Query
// node: current node index in the tree array
// start, end: range covered by the current node
// l, r: the query range [l, r]
// Returns the maximum stack height for a top disc whose height is in [l, r]
int query_segment_tree(int node, int start, int end, int l, int r) {
    // If the query range is completely outside the current segment, or invalid
    if (r < start || end < l || l > r) {
        return 0; // No valid stack found in this range
    }
    // If the current segment is completely within the query range
    if (l <= start && end <= r) {
        return tree[node]; // Return the precomputed maximum for this segment
    }
    // Partially overlapping: recurse on children
    int mid = start + (end - start) / 2; // Use safer mid calculation
    int p1 = query_segment_tree(2 * node, start, mid, l, r);
    int p2 = query_segment_tree(2 * node + 1, mid + 1, end, l, r);
    return std::max(p1, p2);
}

class Solution {
public:
    // Changed parameter types from int N, int r[], int h[] to std::vector<int>& r_vec, std::vector<int>& h_vec
    int maxStackHeight(std::vector<int>& r_vec, std::vector<int>& h_vec) {
        int N = r_vec.size(); // Get N from the vector size
        std::vector<std::pair<int, int>> discs(N);
        for (int i = 0; i < N; ++i) {
            discs[i] = {r_vec[i], h_vec[i]}; // Use vector elements
        }

        // Sort discs:
        // Primary key: radius (r) in ascending order.
        // Secondary key: height (h) in descending order (if r values are equal).
        // This sorting is crucial for the 2D LIS-like problem.
        // It ensures that when processing discs, if r_j = r_i, then h_j >= h_i.
        // This prevents discs with the same radius from being considered for stacking,
        // as the height query (h_k < current_h) will not pick up h_j.
        std::sort(discs.begin(), discs.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.first != b.first) {
                return a.first < b.first;
            }
            return a.second > b.second;
        });

        // Reset the global segment tree for each test case.
        // Size 4 * MAX_H_VAL + 5 is a safe upper bound for segment tree nodes.
        // Initialize with 0s, as no stack has height 0 initially.
        tree.assign(4 * MAX_H_VAL + 5, 0); 

        int max_total_height = 0;
        // This vector stores updates for discs that share the same radius.
        // Updates are applied to the segment tree only after all discs
        // of the current radius group have been processed.
        std::vector<std::pair<int, int>> updates_for_current_r_group;

        for (int i = 0; i < N; ++i) {
            int current_r = discs[i].first;
            int current_h = discs[i].second;

            // Query the segment tree for the maximum stack height
            // whose top-most disc has a height strictly less than current_h.
            // This represents the maximum height of a stack that can be placed below current_h.
            // The query range is [1, current_h - 1]. If current_h is 1, current_h - 1 is 0,
            // and the query function correctly returns 0 for an invalid range.
            int prev_max_stack_height = query_segment_tree(1, 1, MAX_H_VAL, 1, current_h - 1);
            
            // Calculate the total height of the stack ending with current_h as the top-most disc.
            int current_stack_height = current_h + prev_max_stack_height;
            
            // Store this potential update. Updates for discs with the same radius
            // must be applied to the segment tree only after all discs of that radius
            // have been processed. This handles the strict inequality for radius (r_top < r_bottom).
            updates_for_current_r_group.push_back({current_h, current_stack_height});
            
            // Update the overall maximum height found so far.
            max_total_height = std::max(max_total_height, current_stack_height);

            // If this is the last disc, or the next disc has a different radius,
            // it means we have finished processing a group of discs with the same radius.
            // Now, apply all pending updates for this radius group to the segment tree.
            if (i == N - 1 || discs[i].first < discs[i+1].first) {
                for (const auto& p : updates_for_current_r_group) {
                    // p.first is the height of the disc, p.second is the total stack height ending with this disc.
                    update_segment_tree(1, 1, MAX_H_VAL, p.first, p.second);
                }
                updates_for_current_r_group.clear(); // Clear the temporary list for the next group
            }
        }

        return max_total_height;
    }
};
```