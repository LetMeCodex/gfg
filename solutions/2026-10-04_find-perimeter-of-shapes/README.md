# [Perimeter of Shapes in Binary Matrix](https://www.geeksforgeeks.org/problems/find-perimeter-of-shapes/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to calculate the total perimeter of all shapes formed by '1's in a given `n x m` binary matrix. A '1' represents a land cell, and a '0' represents a water cell. The perimeter is defined as the sum of all sides of '1' cells that are adjacent to a '0' cell or to the boundary of the matrix. Shapes are formed by horizontally or vertically connected '1's.

For example, if we have a single '1' cell `[[1]]`, its perimeter is 4 (all four sides are exposed to the boundary). If we have `[[1, 1]]`, the perimeter is 6 (the two '1's share a side, so 3 exposed sides for each, totaling 6).

## Intuition & Approach

The core idea is to iterate through each cell of the matrix and determine its contribution to the total perimeter.

1.  **Initial Contribution**: Every '1' cell, if considered in isolation, has 4 sides. So, a naive approach might be to sum 4 for every '1' encountered.
2.  **Handling Shared Sides**: This naive approach overcounts the perimeter. When two '1' cells are adjacent (horizontally or vertically), they share a common side. This shared side is internal to the shape and should *not* be counted as part of the perimeter.
    *   If we initially add 4 for cell `A` and 4 for cell `B`, and `A` and `B` are adjacent '1's, the shared side has been counted twice (once as a side of `A`, once as a side of `B`).
    *   To correct this, for *each* pair of adjacent '1' cells, we must subtract 2 from our `total_perimeter` (one for the side of `A` that faces `B`, and one for the side of `B` that faces `A`).

3.  **Efficient Counting of Shared Sides**: To avoid double-counting the subtraction (e.g., subtracting for `(i,j)` and `(i,j+1)` when processing `(i,j)`, and then again for `(i,j+1)` and `(i,j)` when processing `(i,j+1)`), we can adopt a systematic approach:
    *   Iterate through each cell `(i, j)` of the matrix.
    *   If `mat[i][j]` is '1':
        *   Add 4 to `total_perimeter` (assuming it's initially isolated).
        *   Check its **right neighbor** `(i, j+1)`: If `(i, j+1)` is within bounds and also contains '1', it means `mat[i][j]` and `mat[i][j+1]` share a side. Subtract 2 from `total_perimeter`.
        *   Check its **bottom neighbor** `(i+1, j)`: If `(i+1, j)` is within bounds and also contains '1', it means `mat[i][j]` and `mat[i+1][j]` share a side. Subtract 2 from `total_perimeter`.

    *   By only checking the right and bottom neighbors, we ensure that each shared side is considered and subtracted exactly once. For example, the shared side between `(i, j)` and `(i, j-1)` would have been handled when `(i, j-1)` was the current cell and `(i, j)` was its right neighbor. Similarly, the shared side between `(i, j)` and `(i-1, j)` would have been handled when `(i-1, j)` was the current cell and `(i, j)` was its bottom neighbor.

This approach correctly accounts for all exposed sides by starting with the maximum possible perimeter and then reducing it for every internal shared boundary.

## Complexity Analysis

-   **Time Complexity**: $O(N \times M)$
    *   We iterate through each cell of the `N \times M` matrix exactly once using nested loops.
    *   Inside the loops, all operations (checking cell value, boundary checks, arithmetic additions/subtractions) are constant time operations.
    *   Therefore, the total time complexity is directly proportional to the number of cells in the matrix.

-   **Space Complexity**: $O(1)$
    *   We use a few integer variables (`n`, `m`, `total_perimeter`, loop counters `i`, `j`) to store dimensions and the running perimeter.
    *   No additional data structures are allocated that scale with the input size `N` or `M`.
    *   Thus, the space complexity is constant.

## Solution Code

```cpp
#include <vector> // Required for std::vector

class Solution {
public:
    // Function signature corrected based on the compilation error and typical GFG problem structure.
    // 'n' and 'm' are now derived from the matrix itself.
    int findPerimeter(std::vector<std::vector<int>>& mat) {
        // Constraints 1 <= n, m <= 1000 ensure mat is not empty and has valid dimensions.
        int n = mat.size();
        int m = mat[0].size(); // Safe to access mat[0] due to constraints (m >= 1)

        int total_perimeter = 0;

        // Iterate through each cell of the matrix
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                // If the current cell contains '1', it contributes to the perimeter
                if (mat[i][j] == 1) {
                    // Initially, assume this '1' cell contributes 4 to the perimeter (its four sides).
                    total_perimeter += 4;

                    // Check the right neighbor: (i, j+1)
                    // If the right neighbor is also '1', they share a common side.
                    // This shared side means two segments of the perimeter are "hidden" (one from current cell, one from neighbor).
                    // So, we subtract 2 from the total perimeter.
                    // We only check right and bottom neighbors to avoid double-counting shared sides.
                    // For example, the shared side between (i,j) and (i,j+1) is handled when processing (i,j).
                    // It won't be handled again when processing (i,j+1) as a left neighbor.
                    if (j + 1 < m && mat[i][j+1] == 1) {
                        total_perimeter -= 2;
                    }

                    // Check the bottom neighbor: (i+1, j)
                    // Similarly, if the bottom neighbor is also '1', they share a common side.
                    // Subtract 2 from the total perimeter.
                    if (i + 1 < n && mat[i+1][j] == 1) {
                        total_perimeter -= 2;
                    }
                }
            }
        }

        return total_perimeter;
    }
};
```