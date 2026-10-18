# [Shortest Path From 1 To n](https://www.geeksforGeeks.org/problems/shortest-path-from-1-to-n0156/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 3 attempt(s)

## Problem Summary

The problem asks us to find the minimum number of steps required to reach a given integer `n`, starting from `1`. We are allowed two types of operations on the current number `x`:
1.  Multiply `x` by 3 (i.e., `x * 3`).
2.  Add 1 to `x` (i.e., `x + 1`).

We need to determine the fewest operations to transform `1` into `n`.

## Intuition & Approach

### Intuition

This problem can be thought of as finding the shortest path in a graph where nodes are numbers and edges are the allowed operations. A Breadth-First Search (BFS) starting from `1` could find the shortest path. However, `n` can be up to $10^5$, and the search space could grow very quickly, potentially leading to Time Limit Exceeded (TLE) or excessive memory usage.

A common strategy for shortest path problems with specific operations is to consider working backward from the target `n` to the starting point `1`. If we reverse the operations:
1.  The reverse of `x * 3` is `x / 3` (if `x` is divisible by 3).
2.  The reverse of `x + 1` is `x - 1`.

So, the problem transforms into finding the minimum steps to reach `1` from `n` using `n / 3` (if `n % 3 == 0`) and `n - 1` operations.

### Greedy Approach (Working Backwards)

When working backward from `n` to `1`, our goal is to reduce `n` as quickly as possible. Let's analyze the two reverse operations:

*   **`n / 3`**: This operation reduces `n` significantly faster than `n - 1`. For example, if `n = 9`, `n / 3 = 3`. If `n = 10`, `n - 1 = 9`.
*   **`n - 1`**: This operation reduces `n` by a small amount.

This observation suggests a greedy strategy:
1.  **If `n` is divisible by 3 (`n % 3 == 0`)**: It is always optimal to perform `n = n / 3`. This is because dividing by 3 brings `n` much closer to `1` in a single step compared to subtracting 1. Any sequence of `n - 1` operations would eventually lead to a multiple of 3, but `n / 3` is a direct, faster path.
2.  **If `n` is not divisible by 3 (`n % 3 != 0`)**: We cannot divide by 3. In this case, the only available operation is `n = n - 1`. We must subtract 1 to change `n` such that it eventually becomes divisible by 3 or reaches `1`.
    *   If `n % 3 == 1` (e.g., `n=4, 7, 10`): Subtracting 1 makes `n` divisible by 3 (e.g., `4 -> 3`, `10 -> 9`).
    *   If `n % 3 == 2` (e.g., `n=2, 5, 8`): Subtracting 1 makes `n` have a remainder of 1 when divided by 3 (e.g., `5 -> 4`, `8 -> 7`). The next step would then be to subtract 1 again to make it divisible by 3 (e.g., `4 -> 3`, `7 -> 6`). Or, if `n=2`, `2 -> 1` directly.

This greedy strategy is optimal because `n/3` is always the most efficient way to reduce `n` when available. If it's not available, `n-1` is the only way to progress towards `1` or make `n` divisible by 3 for a future `n/3` operation.

### Algorithm Steps:

1.  Initialize a `steps` counter to `0`.
2.  Handle the base case: If `n` is `1`, we are already at the target, so return `0` steps.
3.  Enter a `while` loop that continues as long as `n` is greater than `1`.
    a.  Inside the loop, check if `n` is divisible by `3` (`n % 3 == 0`).
        *   If true, update `n = n / 3`.
    b.  Else (if `n` is not divisible by `3`):
        *   Update `n = n - 1`.
    c.  In either case, increment `steps` by `1`.
4.  Once the loop finishes (i.e., `n` becomes `1`), return the total `steps`.

## Complexity Analysis

*   **Time Complexity**: $O(\log_3 n)$
    *   In each iteration of the `while` loop, `n` is either divided by 3 or decremented by 1.
    *   If `n` is divisible by 3, it reduces by a factor of 3.
    *   If `n` is not divisible by 3, it becomes `n-1`. In the worst case (e.g., `n = 3k+2`), it takes two `n-1` operations to reach `3k`, and then one `n/3` operation to reach `k`. So, `n` effectively reduces by a factor of 3 in at most 3 steps.
    *   Since `n` is reduced by a factor of approximately 3 in a constant number of operations, the total number of operations is logarithmic with base 3, i.e., $O(\log_3 n)$. For `n = 10^5`, $\log_3 10^5 \approx 10.4$, so the number of steps is very small.

*   **Space Complexity**: $O(1)$
    *   The algorithm uses a few integer variables (`n`, `steps`) to store the current number and the step count. This auxiliary space remains constant regardless of the input `n`.

## Solution Code

```cpp
class Solution {
public:
    int minimumStep(int n) {
        // Base case: if n is 1, we are already at the destination, 0 steps needed.
        if (n == 1) {
            return 0;
        }

        int steps = 0;
        // We work backwards from n to 1.
        // Each step in reverse (n/3 or n-1) corresponds to a step in forward (i*3 or i+1).
        // We want to reach 1 from n in minimum steps.
        // The greedy strategy is to divide by 3 whenever possible, as it reduces n much faster.
        // If n is not divisible by 3, we must subtract 1 until it becomes divisible by 3,
        // or until it becomes 1.
        while (n > 1) {
            if (n % 3 == 0) {
                // If n is divisible by 3, it's always optimal to divide by 3.
                // This corresponds to a 'current_num * 3' operation in forward.
                n /= 3;
            } else {
                // If n is not divisible by 3, we must subtract 1.
                // This corresponds to a 'current_num + 1' operation in forward.
                // Example: if n = 4 (n % 3 == 1), we do 4 -> 3.
                // Example: if n = 5 (n % 3 == 2), we do 5 -> 4. Then in next iteration, 4 -> 3.
                // This ensures we reach a multiple of 3 or 1.
                n -= 1;
            }
            steps++;
        }
        
        return steps; 
    }
};
```