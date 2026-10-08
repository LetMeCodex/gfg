# [Minimum Operations to Reach n](https://www.geeksforgeeks.org/problems/find-optimum-operation4504/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary
The problem asks us to find the minimum number of operations to reach a target integer `n` starting from 0. The allowed operations are:
1. Add 1 to the current number.
2. Multiply the current number by 2.

## Intuition & Approach
This problem can be effectively solved by thinking backward from the target number `n` to 0. If we want to reach `n` from 0 using "add 1" and "multiply by 2", then when working backward from `n`, the inverse operations are:
1. Subtract 1 from the current number.
2. Divide the current number by 2 (if it's even).

The goal is to reach 0 from `n` using the minimum number of these inverse operations.

Let's consider the choices we have at each step when working backward from `n`:

*   **If `n` is even:** We have two options:
    *   Subtract 1: This would make `n` odd.
    *   Divide by 2: This directly reduces `n` by half.

    It's always more optimal to divide by 2 when `n` is even. Why? Consider reaching an even number `n`. It could have been reached either by `n-1` (adding 1) or `n/2` (multiplying by 2). If we are working backward from `n`, dividing by 2 takes us to `n/2` in one step. If we subtract 1, we get `n-1` (an odd number). To make `n-1` even again, we would have to add 1 (working forward) or subtract 1 (working backward), which would take us to `n-2`. So, to reach `n-2` from `n` working backward, it takes two steps (subtract 1, then subtract 1 again). However, to reach `n/2` from `n` working backward, it takes only one step (divide by 2). Since `n/2` is generally much smaller than `n-2` (for `n > 2`), dividing by 2 is the more efficient choice.

*   **If `n` is odd:** We only have one option:
    *   Subtract 1: This will make `n` even.

    An odd number `n` cannot be reached by multiplying an integer by 2. The only way to reach an odd number `n` (working forward) is from `n-1` by adding 1. Therefore, when working backward from an odd `n`, we must subtract 1 to make it even, allowing for potential division in the next step.

This greedy strategy of prioritizing division by 2 when `n` is even and subtracting 1 when `n` is odd guarantees the minimum number of operations. We continue this process until `n` becomes 0.

The provided solution implements this backward approach:
1. Initialize `operations` to 0.
2. While `n` is greater than 0:
   a. If `n` is even (`n % 2 == 0`), divide `n` by 2 (`n /= 2`).
   b. If `n` is odd, subtract 1 from `n` (`n -= 1`).
   c. Increment the `operations` count.
3. Return the total `operations`.

## Complexity Analysis
- **Time Complexity**: $O(\log n)$
    The dominant operation is division by 2. In each step where `n` is even, we divide `n` by 2. In the worst case, we might subtract 1 to make it even, and then divide by 2. This is similar to how binary representation works. The number of operations is roughly proportional to the number of bits in `n`, which is logarithmic with respect to `n`.

- **Space Complexity**: $O(1)$
    The solution uses a constant amount of extra space for variables like `operations` and `n`. The space used does not grow with the input size.

## Solution Code
```cpp
class Solution {
public:
    /**
     * @brief Calculates the minimum operations to reach n from 0.
     *
     * The allowed operations are adding 1 and multiplying by 2.
     * This function works backward from n to 0 using inverse operations:
     * subtracting 1 and dividing by 2 (if even).
     *
     * @param n The target integer.
     * @return The minimum number of operations required.
     */
    int minOperation(int n) { // Renamed from minOperations to minOperation to match the driver code.
        int operations = 0;
        // We work backward from n to 0.
        // The inverse operations are:
        // 1. Divide by 2 (if n is even)
        // 2. Subtract 1
        while (n > 0) {
            if (n % 2 == 0) {
                // If n is even, it's always optimal to divide by 2.
                // This is because dividing by 2 reduces the number much faster
                // than subtracting 1 (which would make it odd, forcing another subtract 1
                // to make it even again, taking 2 steps to reach n-2).
                n /= 2;
            } else {
                // If n is odd, we must subtract 1.
                // An odd number cannot be reached by doubling an integer.
                // The only way to reach an odd number 'n' is from 'n-1' by adding 1.
                // (This also handles the base case n=1, where n-1=0).
                n -= 1;
            }
            operations++;
        }
        return operations;
    }
};
```