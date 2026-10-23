# [Trace Path](https://www.geeksforgeeks.org/problems/trace-path3840/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to determine if a given path, represented by a string of movement commands ('U' for Up, 'D' for Down, 'L' for Left, 'R' for Right), can be traced entirely within a grid of dimensions `n x m`. The path starts at an arbitrary cell within the grid. We need to return `true` if the path can be contained within the grid, and `false` otherwise.

## Intuition & Approach

The core idea is to understand what it means for a path to be contained within a grid. If a path starts at any cell `(r, c)` within an `n x m` grid, and it moves through a sequence of steps, the entire path must stay within the bounds `0 <= row < n` and `0 <= col < m`.

Instead of trying to find an optimal starting position, we can simplify the problem by considering the path's movement relative to its *own* starting point. Let's assume the path begins at a conceptual origin `(0, 0)`. As we process the movement commands, we can track the current relative row and column displacement from this origin.

While tracking the current position, we also need to keep track of the *extreme* points the path reaches relative to its start. This means we need to find the minimum and maximum row displacements (`min_r`, `max_r`) and the minimum and maximum column displacements (`min_c`, `max_c`) encountered throughout the path.

Once we have these minimum and maximum relative displacements, we can determine the total "span" of the path in both the vertical and horizontal directions.
- The vertical span is `max_r - min_r`.
- The horizontal span is `max_c - min_c`.

For the path to fit within an `n x m` grid, the grid's dimensions must be large enough to accommodate this span.
- If the vertical span is `vertical_span`, it means the path uses `vertical_span + 1` distinct rows (e.g., if `min_r = 0` and `max_r = 1`, the span is 1, and it uses rows 0 and 1, which is 2 rows). Therefore, the grid must have at least `vertical_span + 1` rows. This translates to `n >= vertical_span + 1`, or `n > vertical_span`.
- Similarly, for the horizontal dimension, the grid must have at least `horizontal_span + 1` columns, which means `m >= horizontal_span + 1`, or `m > horizontal_span`.

If both these conditions (`n > vertical_span` and `m > horizontal_span`) are met, then we can always find a starting position within the `n x m` grid such that the entire path stays within bounds. Otherwise, the path is too large for the grid.

**Steps:**
1. Initialize `current_r = 0`, `current_c = 0` to represent the relative position.
2. Initialize `min_r = 0`, `max_r = 0`, `min_c = 0`, `max_c = 0` to track the bounding box of the path relative to its start.
3. Iterate through the movement string `s`:
    - Update `current_r` and `current_c` based on the character ('U', 'D', 'L', 'R').
    - Update `min_r`, `max_r`, `min_c`, `max_c` using `std::min` and `std::max` with the current relative position.
4. Calculate `vertical_span = max_r - min_r`.
5. Calculate `horizontal_span = max_c - min_c`.
6. Return `(n > vertical_span) && (m > horizontal_span)`.

The problem statement mentions the driver code expects a function named `isPossible`. The provided solution implements this.

## Complexity Analysis

- **Time Complexity**: $O(L)$, where $L$ is the length of the input string `s`. We iterate through the string once to process each movement command. The operations inside the loop (updating coordinates and min/max values) take constant time.

- **Space Complexity**: $O(1)$. We only use a few integer variables to store the current position and the minimum/maximum displacements. The space used does not depend on the input string length.