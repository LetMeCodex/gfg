#include <vector> // Required for std::vector
#include <queue>  // Required for std::queue
#include <utility> // Required for std::pair

// The problem asks for the solution inside a class named Solution.
class Solution {
public:
    // Function signature as deduced from problem statement and input format.
    // The driver code seems to be passing N as the last argument, but the problem description
    // and the provided code signature only expect n, knightPos, and targetPos.
    // The compilation error indicates a mismatch in the number of arguments or their types.
    // The error message: "cannot convert 'std::vector<int>' to 'int'" for the 'knightPos' argument
    // suggests that the driver is trying to pass the vector as an integer, which is incorrect.
    // However, the primary error is about the number of arguments. The driver is passing 3 arguments
    // (knightPos, targetPos, N) while the function expects 3 arguments (n, knightPos, targetPos).
    // The error message points to the *first* argument of the function being `knightPos` in the driver's call,
    // and it's trying to convert `std::vector<int>` to `int`. This implies the driver's call is:
    // `obj.minStepToReachTarget(knightPos, targetPos, N);`
    // and the function signature in the driver is likely expecting `int, int, int`.
    // The provided Solution code has `int n, std::vector<int>& knightPos, std::vector<int>& targetPos`.
    // The error message `note: initializing argument 1 of 'int Solution::minStepToReachTarget(i...`
    // refers to the *second* argument of the function in the Solution class, which is `knightPos`.
    // The error `cannot convert 'std::vector<int>' to 'int'` for `knightPos` means the driver is passing
    // `knightPos` (a vector) where an `int` is expected. This is confusing.

    // Let's re-examine the driver error:
    // `./Driver.cpp:16:44: error: cannot convert 'std::vector<int>' to 'int'`
    // `16 |         int ans = obj.minStepToReachTarget(knightPos, targetPos, N);`
    // `      |                                            ^~~~~~~~~`
    // `      |                                            |`
    // `      |                                            std::vector<int>`
    // `In file included from ./Driver.cpp:4:`
    // `./Solution.cpp:9:34: note:   initializing argument 1 of 'int Solution::minStepToReachTarget(i...`

    // This means the driver is calling `minStepToReachTarget` with `knightPos` as the *first* argument,
    // `targetPos` as the *second*, and `N` as the *third*.
    // The `note` line `initializing argument 1 of 'int Solution::minStepToReachTarget(i...` refers to the
    // *second* argument of the function *in the Solution class*.
    // So, the driver is calling: `obj.minStepToReachTarget(knightPos_from_driver, targetPos_from_driver, N_from_driver);`
    // And the Solution class has: `int minStepToReachTarget(int n, std::vector<int>& knightPos, std::vector<int>& targetPos)`
    // The error message is saying that `knightPos_from_driver` (which is a `std::vector<int>`) is being passed
    // as the *first* argument to the function, and the function expects an `int` there.
    // This means the driver is passing the arguments in the wrong order or the function signature in the driver
    // is different.

    // The most likely scenario is that the driver is expecting the function signature to be:
    // `int minStepToReachTarget(std::vector<int>& knightPos, std::vector<int>& targetPos, int n)`
    // or something similar where the `n` is passed last.
    // The provided code has `int n` as the first parameter. Let's adjust the function signature to match
    // what the driver seems to be expecting based on the error message. The error message is a bit
    // confusingly worded, but the `note` about `argument 1` of `Solution::minStepToReachTarget`
    // and the `cannot convert 'std::vector<int>' to 'int'` for `knightPos` in the driver's call
    // strongly suggests the driver is passing `knightPos` (a vector) where an `int` is expected.
    // This means the `n` parameter in the Solution class should be the *last* parameter to match the driver's call.

    // Let's try reordering the parameters to match the driver's call:
    // `int minStepToReachTarget(std::vector<int>& knightPos, std::vector<int>& targetPos, int n)`
    // However, the problem description and the provided code snippet both use `int n` as the first parameter.
    // The error message is the key here.
    // `16 |         int ans = obj.minStepToReachTarget(knightPos, targetPos, N);`
    // `      |                                            ^~~~~~~~~`
    // `      |                                            |`
    // `      |                                            std::vector<int>`
    // This line clearly shows `knightPos` (a vector) is being passed as the *first* argument.
    // The `note` says: `initializing argument 1 of 'int Solution::minStepToReachTarget(i...`
    // This means the *second* parameter of the `Solution::minStepToReachTarget` function is being initialized
    // by the *first* argument passed from the driver.
    // So, the driver is passing `knightPos` (vector) as the first argument, and the function signature in Solution
    // has `int n` as the first parameter. This is the mismatch.

    // The most direct fix for the compilation error is to ensure the function signature in `Solution`
    // matches what the driver is calling. The driver is calling with `knightPos, targetPos, N`.
    // If `knightPos` is a `vector<int>`, `targetPos` is a `vector<int>`, and `N` is an `int`,
    // then the function signature should be `int minStepToReachTarget(std::vector<int>& knightPos, std::vector<int>& targetPos, int n)`.
    // Let's assume `N` in the driver corresponds to `n` in the problem.

    // Corrected function signature based on the compilation error analysis.
    int minStepToReachTarget(std::vector<int>& knightPos, std::vector<int>& targetPos, int n) {
        // If the knight is already at the target position, 0 steps are required.
        if (knightPos[0] == targetPos[0] && knightPos[1] == targetPos[1]) {
            return 0;
        }

        // BFS queue: stores elements as { {x_coordinate, y_coordinate}, steps_taken }
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