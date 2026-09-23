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
        int mid = (start + end) / 2;
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
    int mid = (start + end) / 2;
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
                    update_segment_tree(1, 1, MAX_H_VAL, p.first, p.second);
                }
                updates_for_current_r_group.clear(); // Clear the temporary list for the next group
            }
        }

        return max_total_height;
    }
};