# GeeksforGeeks POTD: Min Steps by Knight

## Problem Link
- [Min Steps by Knight](https://www.geeksforgeeks.org/problems/steps-by-knight5927/1)

## Platform
- GeeksforGeeks (Problem of the Day)

## Difficulty
- Medium

## Solved In
- 2 attempt(s)

---

## Problem Summary

The problem asks us to find the minimum number of moves a knight needs to reach a target square from a given starting square on an N x N chessboard. The knight can move in an 'L' shape: two squares in one direction (horizontally or vertically) and then one square perpendicular to that direction. The board is 1-indexed, meaning coordinates range from 1 to N.

---

## Intuition & Approach

This problem is a classic shortest path problem on a grid. Since we are looking for the *minimum* number of steps, Breadth-First Search (BFS) is the ideal algorithm. BFS explores the grid level by level, guaranteeing that the first time we reach the target square, it will be via the shortest path.

Here's the breakdown of the approach:

1.  **Representing the Board and Knight's Moves**:
    *   The chessboard can be thought of as a graph where each square is a node, and a valid knight's move between two squares represents an edge.
    *   A knight has 8 possible moves from any given square. We can represent these moves using two arrays, `dx` and `dy`, which store the changes in x and y coordinates for each of the 8 moves. For example, `dx = {-2, -2, -1, -1, 1, 1, 2, 2}` and `dy = {-1, 1, -2, 2, -2, 2, -1, 1}`.

2.  **BFS Implementation**:
    *   **Queue**: We'll use a queue to store the states to visit. Each state in the queue will be a `pair` containing:
        *   The current position of the knight (represented as another `pair` of `(x, y)` coordinates).
        *   The number of steps taken to reach this position.
    *   **Visited Set**: To avoid revisiting squares and getting into infinite loops, we need a way to keep track of visited squares. A 2D boolean array `visited[N+1][N+1]` is suitable for this. We use `N+1` for 1-based indexing.
    *   **Initialization**:
        *   Push the starting position of the knight into the queue with 0 steps.
        *   Mark the starting position as visited.
    *   **BFS Loop**:
        *   While the queue is not empty:
            *   Dequeue the current state (position and steps).
            *   If the current position is the target position, we have found the shortest path. Return the number of steps.
            *   For each of the 8 possible knight moves:
                *   Calculate the `next_x` and `next_y` coordinates.
                *   Check if the `(next_x, next_y)` is valid:
                    *   It must be within the board boundaries (1 to N for both coordinates).
                    *   It must not have been visited yet.
                *   If valid, mark `(next_x, next_y)` as visited and enqueue it with `steps + 1`.

3.  **Edge Cases**:
    *   If the knight starts at the target position, the number of steps is 0. This should be checked at the beginning.
    *   The problem statement implies that a path will always exist. If, for some reason, the target is unreachable (which is unlikely for a knight on a standard board), the BFS loop will finish without finding the target. In such a scenario, returning -1 would be appropriate, though it's unlikely to be hit given typical problem constraints.

**Addressing the Compilation Error:**

The provided solution code had a compilation error related to the function signature and how arguments were passed by the driver code. The error message indicated that `std::vector<int>` (likely `knightPos` from the driver) was being passed as the first argument where an `int` was expected. This suggests the driver code was calling the function with arguments in a different order than the `Solution` class's initial signature.

The driver's call: `obj.minStepToReachTarget(knightPos, targetPos, N);`
The original `Solution` signature: `int minStepToReachTarget(int n, std::vector<int>& knightPos, std::vector<int>& targetPos)`

The error message `note: initializing argument 1 of 'int Solution::minStepToReachTarget(i...` combined with `cannot convert 'std::vector<int>' to 'int'` for `knightPos` in the driver's call implies that the driver is passing `knightPos` (a vector) as the *first* argument, and the `Solution` function's *first* parameter is `int n`. This is a mismatch.

The corrected function signature `int minStepToReachTarget(std::vector<int>& knightPos, std::vector<int>& targetPos, int n)` aligns the parameters to match the driver's call order: `knightPos` (vector) maps to the first parameter, `targetPos` (vector) maps to the second, and `N` (int) maps to the third parameter `n`.

---

## Complexity Analysis

*   **Time Complexity**: $O(N^2)$
    *   In the worst case, BFS might visit every square on the N x N board. For each square, we perform a constant number of operations (checking 8 possible moves). Therefore, the time complexity is proportional to the number of cells on the board, which is $N \times N = N^2$.

*   **Space Complexity**: $O(N^2)$
    *   The space complexity is dominated by the `visited` array, which is of size $(N+1) \times (N+1)$, and the BFS queue. In the worst case, the queue can hold up to $O(N^2)$ elements (all cells on the board). Thus, the space complexity is $O(N^2)$.

---

## Solution Code

```cpp
#include <vector> // Required for std::vector
#include <queue>  // Required for std::queue
#include <utility> // Required for std::pair

// The problem asks for the solution inside a class named Solution.
class Solution {
public:
    /**
     * @brief Finds the minimum number of steps a knight needs to reach a target square on an N x N chessboard.
     * 
     * @param knightPos A vector representing the starting position of the knight {x, y}. Coordinates are 1-indexed.
     * @param targetPos A vector representing the target position of the knight {x, y}. Coordinates are 1-indexed.
     * @param n The size of the chessboard (N x N).
     * @return The minimum number of steps required, or -1 if unreachable (though unlikely for this problem).
     */
    // Corrected function signature to match the driver's argument passing order.
    // The driver passes knightPos, targetPos, N.
    int minStepToReachTarget(std::vector<int>& knightPos, std::vector<int>& targetPos, int n) {
        // If the knight is already at the target position, 0 steps are required.
        if (knightPos[0] == targetPos[0] && knightPos[1] == targetPos[1]) {
            return 0;
        }

        // BFS queue: stores elements as { {x_coordinate, y_coordinate}, steps_taken }
        // Using std::pair for nested pairs to store position and steps.
        std::queue<std::pair<std::pair<int, int>, int>> q;

        // Visited array to keep track of cells that have been added to the queue.
        // Using (n + 1) size for 1-based indexing, so indices 1 to n are valid.
        std::vector<std::vector<bool>> visited(n + 1, std::vector<bool>(n + 1, false));

        // Knight's possible moves: 8 directions (dx, dy)
        // These arrays define the change in x and y coordinates for each move.
        // dx: change in x-coordinate
        // dy: change in y-coordinate
        int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
        int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};

        // Push the starting position of the knight into the queue with 0 steps.
        q.push({{knightPos[0], knightPos[1]}, 0});
        // Mark the starting position as visited.
        visited[knightPos[0]][knightPos[1]] = true;

        // Perform BFS
        while (!q.empty()) {
            // Get the current position and steps from the front of the queue.
            std::pair<std::pair<int, int>, int> current = q.front();
            q.pop();

            int curr_x = current.first.first;
            int curr_y = current.first.second;
            int steps = current.second;

            // If the current position is the target position, we have found the shortest path.
            // Return the number of steps taken to reach it.
            if (curr_x == targetPos[0] && curr_y == targetPos[1]) {
                return steps;
            }

            // Explore all 8 possible moves from the current position.
            for (int i = 0; i < 8; ++i) {
                int next_x = curr_x + dx[i];
                int next_y = curr_y + dy[i];

                // Check if the next position is valid:
                // 1. It must be within the board boundaries (1 to n for both x and y).
                // 2. It must not have been visited before to ensure shortest path and avoid cycles.
                if (next_x >= 1 && next_x <= n &&
                    next_y >= 1 && next_y <= n &&
                    !visited[next_x][next_y]) {
                    
                    // Mark the new position as visited.
                    visited[next_x][next_y] = true;
                    // Add the new position to the queue with an incremented step count.
                    q.push({{next_x, next_y}, steps + 1});
                }
            }
        }

        // This line should theoretically not be reached in this problem
        // because a path between any two cells on a chessboard is always assumed to exist
        // for a knight (unless n is very small and specific positions are chosen,
        // but for general n, it's usually reachable).
        // It's a fallback return in case the target is somehow unreachable,
        // which would indicate an issue with problem constraints or understanding.
        return -1; 
    }
};
```