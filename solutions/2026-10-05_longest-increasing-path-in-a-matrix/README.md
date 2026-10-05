# [Longest Increasing Path in Matrix](https://www.geeksforgreeks.org/problems/longest-increasing-path-in-a-matrix/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 3 attempt(s)

## Problem Summary

Given an `n x m` integer matrix, the task is to find the length of the longest increasing path in the matrix.

From each cell, you can move in four directions: up, down, left, or right. You cannot move diagonally or outside the boundary. An increasing path means that each subsequent cell in the path must have a strictly greater value than the current cell.

**Example:**
Input:
```
matrix = [[9,9,4],[6,6,8],[2,1,1]]
```
Output: `4`
Explanation: The longest increasing path is `[1, 2, 6, 9]`. (Path could be `(2,1) -> (2,0) -> (1,0) -> (0,0)`)

## Intuition & Approach

This problem can be solved using Dynamic Programming. A common approach for "longest path" problems on a grid is Depth-First Search (DFS) with memoization. However, the provided solution uses an alternative bottom-up DP approach that leverages sorting.

**Why sorting?**
The core idea behind this approach is to process cells in a specific order that guarantees that when we compute the longest increasing path ending at a cell `(r, c)`, all necessary subproblems (i.e., paths ending at its smaller-valued neighbors) have already been solved.

If we sort all cells of the matrix based on their values in ascending order, we can iterate through them. When we are at a cell `(r, c)` with value `val = matrix[r][c]`, any neighbor `(nr, nc)` with `matrix[nr][nc] < val` would have appeared *earlier* in the sorted list. This means `dp[nr][nc]` (the longest increasing path ending at `(nr, nc)`) would have already been correctly computed. This allows for a straightforward bottom-up DP update.

**Detailed Approach:**

1.  **Initialization**:
    *   Handle the edge case where the matrix is empty (`n=0` or `m=0`), returning `0`.
    *   Create a 2D `dp` table of the same dimensions as the input `matrix`. `dp[i][j]` will store the length of the longest increasing path *ending* at cell `(i, j)`. Initialize all `dp` values to `1`, as every cell itself forms a path of length 1.
    *   Create a list (e.g., `std::vector` of `std::tuple`) to store all cells along with their coordinates: `(value, row, column)`. This list will have `n * m` elements.

2.  **Sort Cells**:
    *   Sort the list of `(value, row, column)` tuples in ascending order based on their `value`. This is the crucial step that enables the bottom-up DP.

3.  **Iterate and Update DP**:
    *   Initialize `overall_max_path = 1` (assuming a non-empty matrix, the minimum path length is 1).
    *   Define arrays for direction vectors (`dr`, `dc`) to easily iterate through the four neighbors (up, down, left, right).
    *   Iterate through each cell `(val, r, c)` in the *sorted* list:
        *   For each of its four neighbors `(nr, nc)`:
            *   Check if `(nr, nc)` is within the matrix boundaries.
            *   If `matrix[nr][nc] < val` (i.e., the neighbor's value is strictly smaller than the current cell's value):
                *   This means we can potentially extend an increasing path from `(nr, nc)` to `(r, c)`.
                *   Since `(nr, nc)` has a smaller value, it was processed earlier in the sorted list, and `dp[nr][nc]` is already correctly computed.
                *   Update `dp[r][c]` by taking the maximum of its current value and `1 + dp[nr][nc]`. This `1` accounts for the current cell `(r, c)`.
        *   After checking all neighbors for the current cell `(r, c)`, update `overall_max_path = std::max(overall_max_path, dp[r][c])`.

4.  **Result**:
    *   After processing all cells in the sorted order, `overall_max_path` will hold the length of the longest increasing path in the matrix. Return this value.

This approach effectively transforms a potentially complex recursive problem into an iterative one by carefully ordering the subproblem computations.

## Complexity Analysis

*   **Time Complexity**: $O(N \cdot M \log(N \cdot M))$
    *   Creating the `cells` list takes $O(N \cdot M)$ time.
    *   Sorting the `cells` list, which contains $N \cdot M$ elements, takes $O(N \cdot M \log(N \cdot M))$ time.
    *   Iterating through the sorted `cells` list: There are $N \cdot M$ cells. For each cell, we iterate through its 4 neighbors. Each neighbor check and DP update is $O(1)$. So, this step takes $O(N \cdot M)$ time.
    *   The dominant factor is the sorting step, leading to an overall time complexity of $O(N \cdot M \log(N \cdot M))$.

*   **Space Complexity**: $O(N \cdot M)$
    *   The `dp` table requires $O(N \cdot M)$ space.
    *   The `cells` list (vector of tuples) stores $N \cdot M$ elements, each taking constant space, resulting in $O(N \cdot M)$ space.
    *   Therefore, the total space complexity is $O(N \cdot M)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // For std::max, std::sort
#include <tuple>     // For std::tuple

class Solution {
public:
    // Renamed function to longIncPath and reordered parameters to match the driver.
    int longIncPath(std::vector<std::vector<int>>& matrix, int n, int m) {
        // Handle empty matrix case: if n or m is 0, no path exists, so return 0.
        if (n == 0 || m == 0) {
            return 0;
        }

        // dp[i][j] will store the length of the longest increasing path ending at cell (i, j).
        // Initialize all dp values to 1, as each cell itself forms a path of length 1.
        std::vector<std::vector<int>> dp(n, std::vector<int>(m, 1));

        // Create a list of all cells, storing their value, row, and column.
        // This allows us to sort cells based on their values.
        std::vector<std::tuple<int, int, int>> cells;
        cells.reserve(n * m); // Pre-allocate memory for efficiency, as size is known.
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cells.emplace_back(matrix[i][j], i, j);
            }
        }

        // Sort the cells based on their values in ascending order.
        // This is crucial for the bottom-up DP approach. It ensures that when we process a cell (r, c),
        // any cell (nr, nc) with matrix[nr][nc] < matrix[r][c] would have already been processed,
        // and its dp value (dp[nr][nc]) would be correctly computed.
        std::sort(cells.begin(), cells.end());

        // Initialize the overall maximum path length found so far.
        // Since each cell is a path of length 1 (if n*m > 0), the minimum possible answer is 1.
        int overall_max_path = 1; 

        // Define directions for moving (up, down, left, right)
        int dr[4] = {-1, 1, 0, 0}; // Row changes for up, down, no change for left, right
        int dc[4] = {0, 0, -1, 1}; // Column changes for no change for up, down, left, right

        // Iterate through the sorted cells
        for (const auto& cell : cells) {
            int val = std::get<0>(cell); // Value of the current cell
            int r = std::get<1>(cell);   // Row index of the current cell
            int c = std::get<2>(cell);   // Column index of the current cell

            // Explore all four possible neighbors of the current cell
            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i]; // Neighbor row
                int nc = c + dc[i]; // Neighbor column

                // Check if the neighbor is within the matrix boundaries
                if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
                    // If the neighbor's value is strictly smaller than the current cell's value,
                    // it means we can potentially form an increasing path from (nr, nc) to (r, c).
                    // Since (nr, nc) has a smaller value, it must have been processed earlier
                    // in the sorted list, so dp[nr][nc] is already correctly computed.
                    if (matrix[nr][nc] < val) {
                        // Update dp[r][c] if a longer path ending at (r, c) is found
                        // by extending a path from (nr, nc).
                        dp[r][c] = std::max(dp[r][c], 1 + dp[nr][nc]);
                    }
                }
            }
            // After checking all neighbors for the current cell (r, c),
            // update the overall maximum path length found across all cells.
            overall_max_path = std::max(overall_max_path, dp[r][c]);
        }

        return overall_max_path;
    }
};
```