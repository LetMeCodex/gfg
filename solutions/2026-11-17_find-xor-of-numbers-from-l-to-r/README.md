# [XOR of Number Range](https://www.geeksforgeeks.org/problems/find-xor-of-numbers-from-l-to-r/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to compute the bitwise XOR sum of all integers in a given range `[l, r]`, inclusive.

## Intuition & Approach

A naive approach would be to iterate from `l` to `r` and XOR each number. However, this would have a time complexity of O(r - l), which can be too slow if the range is large.

The key to an efficient solution lies in recognizing a pattern for the XOR sum of numbers from 0 up to a given number `n`. Let's denote this function as `xor_up_to(n)`.

Consider the XOR sums for small values of `n`:
- `xor_up_to(0) = 0`
- `xor_up_to(1) = 0 ^ 1 = 1`
- `xor_up_to(2) = 0 ^ 1 ^ 2 = 1 ^ 2 = 3`
- `xor_up_to(3) = 0 ^ 1 ^ 2 ^ 3 = 3 ^ 3 = 0`
- `xor_up_to(4) = 0 ^ 1 ^ 2 ^ 3 ^ 4 = 0 ^ 4 = 4`
- `xor_up_to(5) = 0 ^ 1 ^ 2 ^ 3 ^ 4 ^ 5 = 4 ^ 5 = 1`
- `xor_up_to(6) = 0 ^ 1 ^ 2 ^ 3 ^ 4 ^ 5 ^ 6 = 1 ^ 6 = 7`
- `xor_up_to(7) = 0 ^ 1 ^ 2 ^ 3 ^ 4 ^ 5 ^ 6 ^ 7 = 7 ^ 7 = 0`

We can observe a pattern based on `n % 4`:
- If `n % 4 == 0`, `xor_up_to(n) = n`
- If `n % 4 == 1`, `xor_up_to(n) = 1`
- If `n % 4 == 2`, `xor_up_to(n) = n + 1`
- If `n % 4 == 3`, `xor_up_to(n) = 0`

This pattern can be proven by considering the XOR sum of four consecutive numbers. For any integer `k`, the XOR sum `k ^ (k+1) ^ (k+2) ^ (k+3)` is always 0. This is because `k ^ (k+1)` will have the lowest differing bit set, and XORing with `(k+2) ^ (k+3)` will cancel out bits in a predictable way.

Now, to find the XOR sum of the range `[l, r]`, we can use the property of XOR: `A ^ B ^ C = (A ^ B) ^ C`.
The XOR sum of numbers from `l` to `r` can be expressed as:
`(0 ^ 1 ^ ... ^ (l-1) ^ l ^ ... ^ r)` XOR `(0 ^ 1 ^ ... ^ (l-1))`

This simplifies to:
`xor_up_to(r) ^ xor_up_to(l - 1)`

Therefore, the approach is to implement a helper function `xor_up_to(n)` that calculates the XOR sum from 0 to `n` based on the `n % 4` pattern, and then use this helper function to compute `xor_up_to(r) ^ xor_up_to(l - 1)`.

## Complexity Analysis

- **Time Complexity**: $O(1)$
  The `xor_up_to` function performs a constant number of operations (modulo, switch statement, and arithmetic operations). The `findXOR` function calls `xor_up_to` twice and performs a single XOR operation. Thus, the overall time complexity is constant.

- **Space Complexity**: $O(1)$
  The solution uses a fixed amount of memory regardless of the input size, storing only a few variables. Therefore, the space complexity is constant.

## Solution Code

```cpp
class Solution {
public:
    /**
     * @brief Calculates the bitwise XOR of all integers in the range [l, r].
     *
     * This function leverages a helper function `xor_up_to(n)` which calculates
     * the XOR of all integers from 0 to n. The XOR of the range [l, r] can then
     * be computed as `xor_up_to(r) ^ xor_up_to(l - 1)`.
     *
     * The `xor_up_to(n)` function has a pattern based on n % 4:
     * - If n % 4 == 0, the XOR sum is n.
     * - If n % 4 == 1, the XOR sum is 1.
     * - If n % 4 == 2, the XOR sum is n + 1.
     * - If n % 4 == 3, the XOR sum is 0.
     *
     * @param l The lower bound of the range (inclusive).
     * @param r The upper bound of the range (inclusive).
     * @return The bitwise XOR of all integers from l to r.
     */
    int findXOR(int l, int r) {
        // The XOR sum of the range [l, r] is equivalent to
        // (XOR sum from 0 to r) XOR (XOR sum from 0 to l-1).
        return xor_up_to(r) ^ xor_up_to(l - 1);
    }

private:
    /**
     * @brief Calculates the bitwise XOR of all integers from 0 to n.
     *
     * This function utilizes the observed pattern of XOR sums based on n % 4.
     * The pattern is derived from the fact that the XOR sum of any four
     * consecutive integers is 0.
     *
     * @param n The upper bound of the range (inclusive).
     * @return The bitwise XOR of all integers from 0 to n.
     */
    int xor_up_to(int n) {
        switch (n % 4) {
            case 0: return n;      // If n % 4 == 0, XOR sum is n
            case 1: return 1;      // If n % 4 == 1, XOR sum is 1
            case 2: return n + 1;  // If n % 4 == 2, XOR sum is n + 1
            case 3: return 0;      // If n % 4 == 3, XOR sum is 0
        }
        // This part of the code should theoretically not be reached
        // as n % 4 will always result in 0, 1, 2, or 3.
        return 0;
    }
};
```