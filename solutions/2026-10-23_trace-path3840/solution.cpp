#include <string>
#include <algorithm> // Required for std::min and std::max

class Solution {
public:
    // The problem statement does not explicitly provide a function name.
    // Based on common GFG practice and the problem title "Trace Path",
    // a descriptive name like `canTracePath` is appropriate.
    // The driver code expects a function named `isPossible`.
    bool isPossible(int n, int m, std::string s) {
        // Initialize current relative position to (0,0).
        // This represents the current position assuming the starting cell was (0,0).
        int current_r = 0;
        int current_c = 0;

        // Initialize min/max relative displacements from the starting point.
        // These variables will track the overall bounding box of the path
        // relative to its initial (arbitrary) starting point.
        // All paths start at (0,0) relative to themselves, so min/max are initially 0.
        int min_r = 0;
        int max_r = 0;
        int min_c = 0;
        int max_c = 0;

        // Iterate through each movement command in the string 's'.
        for (char move : s) {
            if (move == 'L') {
                current_c--; // Move one cell left: (i, j) to (i, j - 1)
            } else if (move == 'R') {
                current_c++; // Move one cell right: (i, j) to (i, j + 1)
            } else if (move == 'D') {
                current_r--; // Move one cell "up" (row index decreases): (i, j) to (i - 1, j)
            } else if (move == 'U') {
                current_r++; // Move one cell "down" (row index increases): (i, j) to (i + 1, j)
            }

            // After each move, update the minimum and maximum row/column
            // coordinates reached relative to the starting point.
            min_r = std::min(min_r, current_r);
            max_r = std::max(max_r, current_r);
            min_c = std::min(min_c, current_c);
            max_c = std::max(max_c, current_c);
        }

        // Calculate the total vertical and horizontal span of the path.
        // The vertical span is the difference between the maximum and minimum
        // relative row coordinates reached.
        int vertical_span = max_r - min_r;
        // The horizontal span is the difference between the maximum and minimum
        // relative column coordinates reached.
        int horizontal_span = max_c - min_c;

        // For the entire path to remain within the grid, the grid's dimensions
        // must be strictly larger than the path's span in both directions.
        // If the grid has 'n' rows, it can accommodate a path with a vertical span
        // of `vertical_span` if `n > vertical_span`.
        // This is because a path with `vertical_span` requires `vertical_span + 1`
        // distinct row indices (e.g., if min_r=0, max_r=1, span=1, it needs rows 0 and 1).
        // The number of available rows in the grid is `n`. So, we need `n >= (vertical_span + 1)`,
        // which simplifies to `n > vertical_span`.
        // The same logic applies to columns.

        return (n > vertical_span) && (m > horizontal_span);
    }
};