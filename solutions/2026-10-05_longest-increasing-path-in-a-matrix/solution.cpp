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