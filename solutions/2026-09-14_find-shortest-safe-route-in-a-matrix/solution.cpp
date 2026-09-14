#include <vector>
#include <queue>
#include <tuple> // For std::tuple

// The problem asks for the minimum number of steps required to travel.
// Based on the example output, "steps" refers to the number of cells in the path,
// not the number of moves (edges).
// For example, a path of 1 cell (start and end are the same cell) has 1 step.
// A path of 2 cells (1 move) has 2 steps.

class Solution {
public:
    int shortestPath(std::vector<std::vector<int>>& mat) {
        int n = mat.size();
        if (n == 0) return -1;
        int m = mat[0].size();
        if (m == 0) return -1;

        // Step 1: Preprocessing the Grid to identify truly safe cells
        // A cell is unsafe if it contains a landmine (0) or is adjacent to one.
        // Initialize all cells as potentially safe (true).
        std::vector<std::vector<bool>> safe_grid(n, std::vector<bool>(m, true));

        // Directions for 4-directional movement (Up, Down, Left, Right)
        int dr[] = {-1, 1, 0, 0}; // Delta row
        int dc[] = {0, 0, -1, 1}; // Delta column

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (mat[i][j] == 0) { // Found a landmine
                    safe_grid[i][j] = false; // The landmine cell itself is unsafe
                    // Mark its 4 neighbors as unsafe
                    for (int k = 0; k < 4; ++k) {
                        int nr = i + dr[k];
                        int nc = j + dc[k];
                        // Check if neighbor is within grid bounds
                        if (nr >= 0 && nr < n && nc >= 0 && nc < m) {
                            safe_grid[nr][nc] = false;
                        }
                    }
                }
            }
        }

        // Step 2: BFS to find the shortest path
        // State in queue: (row, col, distance)
        // 'distance' here represents the number of cells visited to reach (row, col).
        std::queue<std::tuple<int, int, int>> q;
        // 'visited' array to keep track of visited cells and avoid cycles/reprocessing
        std::vector<std::vector<bool>> visited(n, std::vector<bool>(m, false));

        // Initialize BFS with all safe cells in the leftmost column (column 0)
        for (int r = 0; r < n; ++r) {
            if (safe_grid[r][0]) {
                q.push({r, 0, 1}); // Start at distance 1 (1 cell visited: the starting cell itself)
                visited[r][0] = true;
            }
        }

        // If no safe starting points in the first column, no path exists
        if (q.empty()) {
            return -1;
        }

        while (!q.empty()) {
            auto [r, c, dist] = q.front();
            q.pop();

            // If we reached any cell in the rightmost column (column m-1),
            // this is a valid path. Since BFS explores layer by layer,
            // the first path found to any target cell in the last column will be the shortest.
            if (c == m - 1) {
                return dist; // Return the number of cells in this shortest path
            }

            // Explore 4 neighbors
            for (int k = 0; k < 4; ++k) {
                int nr = r + dr[k];
                int nc = c + dc[k];

                // Check if the neighbor is within grid bounds, is safe, and has not been visited yet
                if (nr >= 0 && nr < n && nc >= 0 && nc < m && 
                    safe_grid[nr][nc] && !visited[nr][nc]) {
                    
                    visited[nr][nc] = true;
                    q.push({nr, nc, dist + 1}); // Increment distance (number of cells visited)
                }
            }
        }

        // If the queue becomes empty and we haven't returned,
        // it means no safe path exists from the leftmost to the rightmost column.
        return -1;
    }
};