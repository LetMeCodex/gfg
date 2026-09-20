# [Largest Subsquare Surrounded by X](https://www.geeksforgreeks.org/problems/largest-subsquare-surrounded-by-x0558/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

Given an `N x N` matrix `mat` consisting of characters 'X' and 'O', the task is to find the side length of the largest square submatrix such that all cells on its perimeter are 'X'. The interior cells of the square can be either 'X' or 'O'. If no such square exists, return 0.

For example, a square of side `k` with top-left corner `(r, c)` is surrounded by 'X's if:
1. All cells from `(r, c)` to `(r, c + k - 1)` are 'X's (top row).
2. All cells from `(r, c)` to `(r + k - 1, c)` are 'X's (left column).
3. All cells from `(r + k - 1, c)` to `(r + k - 1, c + k - 1)` are 'X's (bottom row).
4. All cells from `(r, c + k - 1)` to `(r + k - 1, c + k - 1)` are 'X's (right column).

## Intuition & Approach

A naive approach would be to iterate through all possible top-left corners `(i, j)` and for each, iterate through all possible side lengths `k`. For each `(i, j, k)` combination, we would then check if all four sides of the square are composed entirely of 'X's. Checking the perimeter of a square of side `k` takes `O(k)` time. This would lead to an `O(N^4)` time complexity, which is too slow for `N` up to 1000.

To optimize this, we can use dynamic programming to precompute information about consecutive 'X's.

**1. Precomputation using DP:**
We can create two auxiliary DP tables:
- `dp_right[i][j]`: Stores the count of consecutive 'X's starting at `(i, j)` and extending to the right (inclusive of `(i, j)`).
- `dp_down[i][j]`: Stores the count of consecutive 'X's starting at `(i, j)` and extending downwards (inclusive of `(i, j)`).

These tables can be filled by iterating from the bottom-right corner of the matrix to the top-left corner.
- If `mat[i][j] == 'X'`:
    - `dp_right[i][j] = 1 + (j + 1 < n ? dp_right[i][j+1] : 0)`
    - `dp_down[i][j] = 1 + (i + 1 < n ? dp_down[i+1][j] : 0)`
- If `mat[i][j] == 'O'`:
    - `dp_right[i][j] = 0`
    - `dp_down[i][j] = 0`

This precomputation takes `O(N^2)` time and `O(N^2)` space.

**2. Finding the Largest Subsquare:**
After precomputing `dp_right` and `dp_down`, we can iterate through all possible top-left corners `(i, j)` of a potential square submatrix.
- For each `(i, j)`:
    - If `mat[i][j] == 'O'`, it cannot be the top-left corner of an 'X'-surrounded square, so we skip it.
    - Determine the maximum possible side length `k` for a square starting at `(i, j)`. This is limited by the available 'X's to the right and downwards from `(i, j)`. So, `k` can be at most `std::min(dp_right[i][j], dp_down[i][j])`. Let's call this `max_k_for_current_cell`.
    - Now, iterate `k` downwards from `max_k_for_current_cell` down to `1`. We iterate downwards because we are looking for the *largest* `k`, so the first valid `k` we find for a given `(i, j)` will be the maximum for that `(i, j)`.
    - **Optimization**: If the current `k` is not greater than the `max_side` found so far, we can `break` from this inner loop. There's no need to check smaller `k` values for the current `(i, j)` if they won't yield a larger overall square.
    - For a square of side `k` with top-left `(i, j)`, its bottom-right corner would be `(i + k - 1, j + k - 1)`. We need to check if its bottom and right sides are also composed of `k` consecutive 'X's:
        - **Bottom side check**: The segment from `(i + k - 1, j)` to `(i + k - 1, j + k - 1)` must be 'X's. This can be checked using `dp_right[i + k - 1][j] >= k`.
        - **Right side check**: The segment from `(i, j + k - 1)` to `(i + k - 1, j + k - 1)` must be 'X's. This can be checked using `dp_down[i][j + k - 1] >= k`.
    - If both conditions are met, then a square of side `k` is valid. Update `max_side = k` and `break` from the inner `k` loop (as we found the largest `k` for this `(i, j)`).

The overall algorithm proceeds in two main phases:
1.  **Precomputation**: Fill `dp_right` and `dp_down` tables.
2.  **Verification**: Iterate through all possible top-left corners and potential side lengths, using the precomputed tables to efficiently check perimeter conditions.

This approach reduces the time complexity significantly.

## Complexity Analysis

-   **Time Complexity**:
    -   Precomputing `dp_right` and `dp_down` tables takes `O(N^2)` time, as we iterate through each cell of the `N x N` matrix once.
    -   The second phase involves iterating through all possible top-left corners `(i, j)` (`N^2` combinations). For each `(i, j)`, we iterate `k` from `min(dp_right[i][j], dp_down[i][j])` down to `1`. In the worst case, `k` can be up to `N`. This gives an `O(N)` iteration for `k`. Inside the `k` loop, operations are constant time.
    -   Therefore, the second phase takes `O(N * N * N) = O(N^3)` time.
    -   The overall time complexity is dominated by the second phase, resulting in **$O(N^3)$**.

-   **Space Complexity**:
    -   We use two auxiliary `N x N` DP tables, `dp_right` and `dp_down`. Each stores integers.
    -   This requires `O(N^2)` space.
    -   The overall space complexity is **$O(N^2)$**.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // For std::min

class Solution {
public:
    int largestSubsquare(std::vector<std::vector<char>>& mat) {
        // Get the size n from the matrix itself.
        int n = mat.size();

        // Handle edge case for an empty matrix or n=0.
        if (n == 0) {
            return 0;
        }

        // dp_right[i][j]: Stores the count of consecutive 'X's starting at (i, j) and extending to the right.
        // dp_down[i][j]: Stores the count of consecutive 'X's starting at (i, j) and extending downwards.
        // Initialize with 0s.
        std::vector<std::vector<int>> dp_right(n, std::vector<int>(n, 0));
        std::vector<std::vector<int>> dp_down(n, std::vector<int>(n, 0));

        // Precompute dp_right and dp_down tables.
        // We iterate from bottom-right to top-left to correctly calculate consecutive 'X's.
        for (int i = n - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                if (mat[i][j] == 'X') {
                    // If the current cell is 'X', it contributes 1 to the count.
                    // Add the count from the adjacent cell (to the right for dp_right, downwards for dp_down).
                    
                    // Calculate dp_right[i][j]
                    dp_right[i][j] = 1;
                    if (j + 1 < n) { // If there's a cell to the right
                        dp_right[i][j] += dp_right[i][j+1];
                    }

                    // Calculate dp_down[i][j]
                    dp_down[i][j] = 1;
                    if (i + 1 < n) { // If there's a cell downwards
                        dp_down[i][j] += dp_down[i+1][j];
                    }
                }
                // If mat[i][j] == 'O', dp_right[i][j] and dp_down[i][j] remain 0,
                // which is correct as 'O' breaks any sequence of 'X's.
            }
        }

        int max_side = 0; // This will store the side length of the largest valid square found.

        // Iterate over all possible top-left corners (i, j) of a potential square submatrix.
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // If the current cell mat[i][j] is 'O', it cannot be the top-left corner
                // of any square submatrix surrounded by 'X's, so we skip it.
                if (mat[i][j] == 'O') {
                    continue;
                }

                // Determine the maximum possible side length 'k' for a square starting at (i, j).
                // This is limited by the number of consecutive 'X's available to the right and downwards
                // from the current cell (i, j).
                int max_k_for_current_cell = std::min(dp_right[i][j], dp_down[i][j]);
                
                // Iterate 'k' (side length) downwards from the maximum possible 'k' for this (i, j) down to 1.
                // We iterate downwards because we want the largest 'k', so the first valid 'k' we find
                // will be the largest for this specific (i, j).
                for (int k = max_k_for_current_cell; k >= 1; --k) {
                    // Optimization: If the current 'k' is not greater than the largest 'max_side' found so far,
                    // there's no need to check this 'k' or any smaller 'k' values for this (i, j).
                    // This is because we are looking for the overall largest square, and we've already found one
                    // that is at least as large as 'k'.
                    if (k <= max_side) {
                        break; 
                    }

                    // Check if the bottom row and the right column of the potential square of side 'k'
                    // are also completely surrounded by 'X's.
                    // The bottom-right corner of this potential square would be (i + k - 1, j + k - 1).
                    
                    // 1. Check the bottom row segment: It starts at (i + k - 1, j) and needs to have 'k' 'X's.
                    //    This is verified by checking if dp_right[i + k - 1][j] is at least 'k'.
                    // 2. Check the right column segment: It starts at (i, j + k - 1) and needs to have 'k' 'X's.
                    //    This is verified by checking if dp_down[i][j + k - 1] is at least 'k'.
                    if (dp_right[i + k - 1][j] >= k && dp_down[i][j + k - 1] >= k) {
                        // If all four sides (top, left, bottom, right) are surrounded by 'X's,
                        // then this square of side 'k' is valid.
                        max_side = k; // Update the overall maximum side length found.
                        break;        // Found the largest 'k' for this specific (i, j), so we can stop
                                      // checking smaller 'k's for this (i, j) and move to the next (i, j).
                    }
                }
            }
        }

        return max_side; // Return the side length of the largest square submatrix found.
    }
};
```