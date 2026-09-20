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