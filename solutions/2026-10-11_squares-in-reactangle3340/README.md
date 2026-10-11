# Minimum Squares with Full Cuts

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 1 attempt(s)

## Problem Summary

Given a rectangle of dimensions `n x m`, we need to find the minimum number of squares that can be formed by cutting the rectangle. The constraint is that each cut must be made completely across the remaining paper, either horizontally or vertically.

## Intuition & Approach

This problem can be solved using a greedy approach that is remarkably similar to the Euclidean algorithm for finding the Greatest Common Divisor (GCD).

The core idea is to repeatedly cut out the largest possible square(s) from the current rectangular piece.

1.  **Base Cases**:
    *   If either dimension (`n` or `m`) is 0, it means there's no paper left, so we need 0 squares.
    *   If `n` and `m` are equal, the rectangle is already a square. We need just 1 square (the rectangle itself).

2.  **Greedy Step**:
    *   Assume, without loss of generality, that `n` is the smaller dimension and `m` is the larger dimension. If `n > m`, we can simply swap them.
    *   From an `n x m` rectangle (where `n <= m`), the largest square we can cut is an `n x n` square.
    *   We can cut `m / n` (integer division) such `n x n` squares from the `n x m` rectangle.
    *   After cutting these `m / n` squares, the remaining piece of paper will have dimensions `n x (m % n)`.
    *   The total number of squares will be the sum of the `m / n` squares we just cut, plus the minimum number of squares required to tile the remaining `n x (m % n)` rectangle.

This process is recursive. We call the `minSquares` function again with the dimensions of the remaining rectangle (`n` and `m % n`). The recursion continues until one of the dimensions becomes 0, at which point the base case returns 0, and the results are summed up.

This greedy strategy works because by always cutting the largest possible square, we are effectively reducing the problem to a smaller, similar subproblem. The Euclidean algorithm's efficiency stems from this same principle of reducing the problem size by taking remainders.

## Complexity Analysis

*   **Time Complexity**: $O(\log(\min(n, m)))$
    The time complexity is logarithmic with respect to the smaller of the two dimensions (`n` and `m`). This is because, in each recursive step, the dimensions are reduced in a manner similar to the Euclidean algorithm for GCD. The larger dimension is effectively replaced by its remainder when divided by the smaller dimension, which significantly reduces the problem size in each step.

*   **Space Complexity**: $O(\log(\min(n, m)))$
    The space complexity is determined by the depth of the recursion stack. Since the recursion depth is logarithmic with respect to the smaller dimension (similar to the time complexity), the space complexity is also $O(\log(\min(n, m)))$.

## Solution Code

```cpp
#include <algorithm> // Required for std::swap

class Solution {
public:
    /**
     * @brief Finds the minimum number of squares that can be formed by cutting a rectangular paper.
     * Each cut must be made completely across the remaining paper, either horizontally or vertically.
     * This problem follows a greedy strategy similar to the Euclidean algorithm for GCD.
     * We repeatedly cut out the largest possible square(s) from the current rectangle.
     *
     * @param n The length of one side of the rectangular paper.
     * @param m The length of the other side of the rectangular paper.
     * @return The minimum number of squares.
     */
    long long minSquares(long long n, long long m) {
        // Base case 1: If either dimension is 0, no paper is left, so 0 squares are formed.
        if (n == 0 || m == 0) {
            return 0;
        }
        
        // Base case 2: If both dimensions are equal, the paper is already a square.
        // We need 1 square (the paper itself).
        if (n == m) {
            return 1;
        }
        
        // To simplify the logic, ensure 'n' is always the smaller dimension.
        // This allows us to consistently think about cutting 'n x n' squares.
        if (n > m) {
            std::swap(n, m);
        }
        
        // Calculate how many squares of size 'n x n' can be cut from the 'm' side.
        // This is equivalent to integer division: m / n.
        long long num_n_by_n_squares = m / n;
        
        // After cutting 'num_n_by_n_squares' squares of size 'n x n',
        // the remaining paper will have dimensions 'n x (m % n)'.
        // The total number of squares is the sum of the squares just cut
        // and the squares obtained from the remaining piece of paper.
        return num_n_by_n_squares + minSquares(n, m % n);
    }
};
```