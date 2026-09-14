# [Shortest Safe Route in Grid](https://www.geeksforgreeks.org/problems/find-shortest-safe-route-in-a-matrix/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 1 attempt(s)

## Problem Summary

Given a binary matrix `mat` where `1` represents a safe cell and `0` represents a landmine. We need to find the shortest path from any cell in the first column to any cell in the last column. The path can only consist of safe cells. A cell is considered unsafe if it contains a landmine (`0`) or if it is adjacent (horizontally or vertically) to a landmine. Movement is restricted to 4 directions (up, down, left, right). The "shortest path" refers to the minimum number of cells visited in the path. If no such path exists, return -1.

## Intuition & Approach

The problem asks for the shortest path in an unweighted grid, which immediately suggests using Breadth-First Search (BFS). However, there's a crucial preprocessing step required due to the special definition of "unsafe" cells.

### Intuition

1.  **Identifying Truly Safe Cells**: The primary challenge is that not only cells with `0` are unsafe, but also their immediate 4-directional neighbors. This means we cannot directly use the input `mat` for pathfinding. We need to first create a derived grid that accurately marks all truly safe cells.
2.  **Shortest Path with BFS**: Once we have the truly safe cells, the problem becomes a standard shortest path on an unweighted grid. BFS is ideal because it explores the grid layer by layer, guaranteeing that the first time we reach a target cell, it's via the shortest possible path.
3.  **Multiple Starting Points, Multiple End Points**: We can start from *any* safe cell in the first column and need to reach *any* cell in the last column. BFS naturally handles multiple starting points by initializing the queue with all valid starting positions. The first time BFS reaches *any* cell in the last column, that path will be the shortest overall.

### Approach

1.  **Preprocessing the Grid (`safe_grid`)**:
    *   Create a boolean matrix `safe_grid` of the same dimensions as `mat`, initially marking all cells as `true`.
    *   Iterate through the original `mat`.
    *   If `mat[i][j]` is `0` (a landmine):
        *   Mark `safe_grid[i][j]` as `false`.
        *   Also, mark its four 4-directional neighbors (up, down, left, right), if they are within grid bounds, as `false` in `safe_grid`.
    *   After this step, `safe_grid[r][c]` will be `true` only if `(r, c)` is a safe cell that can be part of a valid path.

2.  **Breadth-First Search (BFS)**:
    *   Initialize a queue to store tuples of `(row, col, distance)`. `distance` here represents the number of cells visited to reach `(row, col)` from a starting point.
    *   Initialize a `visited` boolean matrix to keep track of cells already processed by BFS, preventing cycles and redundant work.
    *   **BFS Initialization**: Iterate through each row `r` in the first column (`c = 0`).
        *   If `safe_grid[r][0]` is `true` (meaning it's a valid starting point):
            *   Add `(r, 0, 1)` to the queue. The distance is `1` because the starting cell itself counts as one step in the path.
            *   Mark `visited[r][0]` as `true`.
    *   If the queue is empty after this initialization (no safe starting points in the first column), return -1 immediately.
    *   **BFS Loop**:
        *   While the queue is not empty:
            *   Dequeue the front element `(r, c, dist)`.
            *   **Goal Check**: If `c == m - 1` (we have reached a cell in the last column), then `dist` is the shortest path length. Return `dist`.
            *   **Explore Neighbors**: For each of the four possible neighbors `(nr, nc)` of `(r, c)`:
                *   Check if `(nr, nc)` is within grid bounds.
                *   Check if `safe_grid[nr][nc]` is `true` (it's a truly safe cell).
                *   Check if `visited[nr][nc]` is `false` (it hasn't been visited yet in this BFS).
                *   If all conditions are met:
                    *   Mark `visited[nr][nc]` as `true`.
                    *   Enqueue `(nr, nc, dist + 1)`.
    *   If the BFS loop finishes and we haven't returned a distance (meaning the last column was never reached), it implies no safe path exists. Return -1.

This approach ensures that we only traverse valid cells and find the shortest path efficiently.

## Complexity Analysis

Let `N` be the number of rows and `M` be the number of columns in the grid.

-   **Time Complexity**: $O(N \cdot M)$
    *   **Preprocessing Step**: We iterate through the entire `N x M` grid once to identify landmines and their adjacent unsafe cells. For each cell, we perform constant time operations (checking its value, updating `safe_grid` for up to 5 cells). This takes $O(N \cdot M)$ time.
    *   **BFS Step**: In the worst case, BFS visits each cell in the grid at most once. For each visited cell, it performs constant time operations (dequeuing, checking up to 4 neighbors, enqueuing). Therefore, the BFS part also takes $O(N \cdot M)$ time.
    *   Combining both steps, the total time complexity is $O(N \cdot M) + O(N \cdot M) = O(N \cdot M)$.

-   **Space Complexity**: $O(N \cdot M)$
    *   **`safe_grid`**: A boolean matrix of size `N x M` is used to store the truly safe cells. This requires $O(N \cdot M)$ space.
    *   **`visited`**: A boolean matrix of size `N x M` is used to keep track of visited cells during BFS. This requires $O(N \cdot M)$ space.
    *   **Queue**: In the worst case (e.g., a grid full of safe cells, or a very wide layer in BFS), the queue can hold up to $O(N \cdot M)$ elements. Each element stores three integers. This requires $O(N \cdot M)$ space.
    *   Combining these, the total space complexity is $O(N \cdot M)$.

## Solution Code

```cpp
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
```