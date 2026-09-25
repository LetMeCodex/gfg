# [Box Stacking](https://www.geeksforgeeks.org/problems/box-stacking/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to find the maximum possible height of a stack of boxes, given the dimensions (height, width, length) of `N` different types of boxes. The rules for stacking are:
1.  A box can only be placed on top of another box if its base dimensions (width and length) are *strictly smaller* than the base dimensions of the box below it.
2.  Boxes can be rotated. This means any of the three dimensions can serve as the height, with the other two forming the base.

We need to find the maximum total height achievable by stacking any number of boxes, including using multiple boxes of the same type (though the problem statement implies distinct boxes, the typical interpretation for such problems allows using multiple instances of the *same type* of box, but here it's `N` distinct boxes, so each box is unique). The solution code implies we are given `N` distinct boxes and we can use each box at most once, but in any orientation. The standard interpretation of "Box Stacking" problems usually allows using multiple instances of the *same type* of box. Given the input `vector<int>& height, vector<int>& width, vector<int>& length`, it implies `N` distinct physical boxes, each with its own set of dimensions. We can use each of these `N` boxes in one of its three orientations at most once in a stack.

## Intuition & Approach

This problem is a classic dynamic programming problem that shares similarities with the Longest Increasing Subsequence (LIS) problem, but extended to 2D base dimensions and with the added complexity of box rotations.

Here's the breakdown of the approach:

1.  **Generate All Possible Orientations:**
    Since each box can be rotated, a single physical box `(h, w, l)` can be oriented in three ways, where each dimension takes a turn being the height, and the other two form the base. To ensure consistent comparison of base dimensions, we always store the base dimensions in a canonical order (e.g., `min(dim1, dim2)` and `max(dim1, dim2)`).
    For each original box `(H, W, L)`:
    *   Orientation 1: Height `H`, Base `(min(W, L), max(W, L))`
    *   Orientation 2: Height `W`, Base `(min(H, L), max(H, L))`
    *   Orientation 3: Height `L`, Base `(min(H, W), max(H, W))`
    We collect all these `3N` possible box orientations into a single list or vector. Each entry in this list will represent a specific box with a specific height and a specific pair of base dimensions.

2.  **Sort the Boxes:**
    To apply a dynamic programming approach similar to LIS, we need to process the boxes in a specific order. A common and effective strategy for this problem is to sort all the generated box orientations by their base area in *descending* order. If two boxes have the same base area, their relative order doesn't strictly matter for correctness, but sorting by one of the base dimensions (e.g., width) in descending order can be a tie-breaker. The provided solution sorts purely by `(a.w * a.l) > (b.w * b.l)`, which is base area in descending order. This ensures that when we consider placing a box `i` on top of a box `j` (where `j` comes before `i` in the sorted list), box `j` is generally "larger" or at least not "smaller" in terms of base area, making it a potential candidate to be below box `i`.

3.  **Dynamic Programming (LIS-like):**
    We use a `dp` array, where `dp[i]` will store the maximum height of a stack that can be formed with `all_orientations[i]` as the topmost box.
    *   Initialize `dp[i]` with `all_orientations[i].h` for all `i`. This represents a stack consisting of only that single box.
    *   Iterate `i` from `0` to `num_orientations - 1` (considering each box as a potential top of a stack).
    *   For each `all_orientations[i]`, iterate `j` from `0` to `i - 1` (considering all boxes that come before `i` in the sorted list as potential boxes *below* `i`).
    *   Check if `all_orientations[i]` can be placed on top of `all_orientations[j]`. This condition is met if `all_orientations[i].w < all_orientations[j].w` AND `all_orientations[i].l < all_orientations[j].l`.
    *   If the condition is met, it means we can potentially extend the stack ending with `all_orientations[j]` by placing `all_orientations[i]` on top. Update `dp[i] = max(dp[i], dp[j] + all_orientations[i].h)`.
    *   Keep track of the overall maximum value found in the `dp` array throughout this process. This maximum value will be our final answer.

The `min/max` normalization for base dimensions (`min(w,l), max(w,l)`) is crucial. It ensures that if a box has base dimensions `(A, B)`, it's treated the same as `(B, A)`. When comparing `box_i` and `box_j`, we then simply check `box_i.w < box_j.w` and `box_i.l < box_j.l`. Since `w` is always the smaller base dimension and `l` is always the larger (due to `min/max` normalization), this comparison correctly checks if `box_i`'s base fits strictly inside `box_j`'s base.

## Complexity Analysis

Let `N` be the number of initial boxes.

-   **Time Complexity**:
    *   **Generating orientations**: For each of the `N` boxes, we generate 3 orientations. This takes `O(N)` time.
    *   **Sorting**: We have `3N` box orientations. Sorting them takes `O(3N log(3N))`, which simplifies to `O(N log N)`.
    *   **Dynamic Programming**: We iterate through `3N` boxes (`i` loop) and for each, we iterate through up to `3N` previous boxes (`j` loop). This results in `O((3N)^2)` operations, which simplifies to `O(N^2)`.
    *   **Overall**: The dominant factor is the DP step. Thus, the total time complexity is $O(N^2)$.

-   **Space Complexity**:
    *   **`all_orientations` vector**: Stores `3N` `Box` structs. This requires `O(3N)` space, which simplifies to `O(N)`.
    *   **`dp` vector**: Stores `3N` `long long` values. This requires `O(3N)` space, which simplifies to `O(N)`.
    *   **Overall**: The total space complexity is $O(N)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm>
#include <tuple> // Not strictly used in the provided code, but often useful for tuples

using namespace std;

// Structure to represent a box with its height, width, and length
struct Box {
    long long h, w, l;
};

// Custom comparison function for sorting boxes
// Sorts boxes by their base area (w * l) in descending order.
bool compareBoxes(const Box& a, const Box& b) {
    return (a.w * a.l) > (b.w * b.l);
}

class Solution {
public:
    long long maxHeight(vector<int>& height, vector<int>& width, vector<int>& length) {
        vector<Box> all_orientations;
        int n = height.size();

        // Step 1: Generate all possible orientations for each box
        for (int i = 0; i < n; ++i) {
            long long h = height[i];
            long long w = width[i];
            long long l = length[i];

            // For each box (h, w, l), generate 3 orientations:
            // 1. h as height, (w, l) as base. Normalize base dimensions (min, max).
            all_orientations.push_back({h, min(w, l), max(w, l)});
            // 2. w as height, (h, l) as base. Normalize base dimensions (min, max).
            all_orientations.push_back({w, min(h, l), max(h, l)});
            // 3. l as height, (h, w) as base. Normalize base dimensions (min, max).
            all_orientations.push_back({l, min(h, w), max(h, w)});
        }

        // Step 2: Sort all generated box orientations
        // Sort by base area (w * l) in descending order.
        // This helps in the DP step, as larger base boxes come first.
        sort(all_orientations.begin(), all_orientations.end(), compareBoxes);

        int num_orientations = all_orientations.size();
        // dp[i] stores the maximum height of a stack ending with all_orientations[i]
        vector<long long> dp(num_orientations);
        long long max_h = 0; // Stores the overall maximum height found

        // Step 3: Dynamic Programming (LIS-like approach)
        for (int i = 0; i < num_orientations; ++i) {
            // Initialize dp[i] with the height of the current box itself
            // (a stack with just one box).
            dp[i] = all_orientations[i].h;

            // Iterate through previous boxes (j < i) to find potential boxes to stack on
            for (int j = 0; j < i; ++j) {
                // Check if box 'i' can be placed on top of box 'j'
                // Condition: box 'i's base dimensions must be strictly smaller than box 'j's base dimensions.
                // Due to min/max normalization, w is always the smaller base dim and l is the larger.
                if (all_orientations[i].w < all_orientations[j].w && 
                    all_orientations[i].l < all_orientations[j].l) {
                    
                    // If it can be stacked, update dp[i] if a taller stack is formed
                    dp[i] = max(dp[i], dp[j] + all_orientations[i].h);
                }
            }
            // Update the overall maximum height found so far
            max_h = max(max_h, dp[i]);
        }

        return max_h;
    }
};
```