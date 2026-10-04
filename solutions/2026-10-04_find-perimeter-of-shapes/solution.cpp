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