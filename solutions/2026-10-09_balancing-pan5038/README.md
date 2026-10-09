# [Balancing with Distinct Powers](https://www.geeksforg eeks.org/problems/balancing-pan5038/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks whether a target weight `b` can be balanced on a scale using a set of weights that are distinct powers of an integer `a`. We are given `a` and `b`. The available weights are $a^0, a^1, a^2, \dots$. For each power $a^k$, we have three options:
1. Place $a^k$ on the same pan as `b`.
2. Place $a^k$ on the pan opposite to `b`.
3. Do not use $a^k$ at all.

We need to determine if there exists a combination of these choices such that the scale balances.

Mathematically, if `b` is on the left pan, and we place some powers of `a` on the left pan and others on the right pan, the balance equation would be:
`b + (sum of a^i on left pan) = (sum of a^j on right pan)`

Rearranging this equation, we get:
`b = (sum of a^j on right pan) - (sum of a^i on left pan)`

This means `b` must be representable as a sum `sum(c_k * a^k)`, where each coefficient `c_k` can be:
- `1`: if $a^k$ is placed on the right pan (opposite to `b`).
- `-1`: if $a^k$ is placed on the left pan (same as `b`).
- `0`: if $a^k$ is not used.

Each power $a^k$ can be used at most once (i.e., each `c_k` corresponds to a distinct power).

## Intuition & Approach

The problem essentially asks if `b` can be represented in a "balanced base-`a`" system, where the digits (coefficients) can be `{-1, 0, 1}`. This is similar to how numbers are represented in a standard base system, but with a different set of allowed digits.

We can determine the coefficients `c_k` greedily, starting from the lowest power $a^0$ (i.e., `c_0`), then $a^1$ (`c_1`), and so on.

Consider the equation:
`b = c_0 * a^0 + c_1 * a^1 + c_2 * a^2 + ...`

To find `c_0`, we can take the equation modulo `a`:
`b % a = (c_0 * a^0 + c_1 * a^1 + c_2 * a^2 + ...) % a`
Since $a^1, a^2, \dots$ are all multiples of `a`, their terms become `0` modulo `a`.
So, `b % a = c_0 % a`.

Now, we check the possible values for `c_0` (which are `-1, 0, 1`) and how they relate to `b % a`:
1.  **If `c_0 = 0`**: Then `b % a` must be `0`. If `b % a == 0`, we choose `c_0 = 0`. The remaining value to balance for higher powers is `(b - 0 * a^0) / a = b / a`.
2.  **If `c_0 = 1`**: Then `b % a` must be `1`. If `b % a == 1`, we choose `c_0 = 1`. The remaining value to balance is `(b - 1 * a^0) / a = (b - 1) / a`.
3.  **If `c_0 = -1`**: Then `b % a` must be `-1 % a`. In modular arithmetic, `-1 % a` is equivalent to `(a - 1) % a`. If `b % a == a - 1`, we choose `c_0 = -1`. The remaining value to balance is `(b - (-1) * a^0) / a = (b + 1) / a`.

If `b % a` is any other value (i.e., not `0`, `1`, or `a-1`), it's impossible to choose a `c_0` from `{-1, 0, 1}` that satisfies `b % a = c_0 % a`. In this case, `b` cannot be balanced, and we return `false`.

We repeat this process in a `while` loop:
- In each iteration, `b` represents the remaining value to be balanced.
- We calculate `remainder = b % a`.
- Based on `remainder`, we determine the coefficient for the current power of `a` (effectively $a^k$ for increasing `k`).
- We update `b` to `(b - c_k * a^k) / a`, which simplifies to `(b - c_k) / a` because we are effectively dividing the entire equation by `a` to shift to the next power.
- The loop continues until `b` becomes `0`. If `b` reaches `0`, it means we have successfully represented the original target weight using the allowed coefficients, and we return `true`.

This greedy strategy works because the choice for the current coefficient `c_k` only depends on the current `b % a`, and it correctly propagates the remaining value to the next higher power.

**Example**: `a = 3, b = 7`
1.  `b = 7`. `remainder = 7 % 3 = 1`.
    Since `remainder == 1`, choose `c_0 = 1`. Update `b = (7 - 1) / 3 = 2`.
    (Equation so far: `7 = 1 * 3^0 + ...`)
2.  `b = 2`. `remainder = 2 % 3 = 2`.
    Since `remainder == a - 1` (`2 == 3 - 1`), choose `c_1 = -1`. Update `b = (2 + 1) / 3 = 1`.
    (Equation so far: `7 = 1 * 3^0 - 1 * 3^1 + ...`)
3.  `b = 1`. `remainder = 1 % 3 = 1`.
    Since `remainder == 1`, choose `c_2 = 1`. Update `b = (1 - 1) / 3 = 0`.
    (Equation so far: `7 = 1 * 3^0 - 1 * 3^1 + 1 * 3^2`)
4.  `b = 0`. The loop terminates. Return `true`.
    Verification: `1 * 3^0 - 1 * 3^1 + 1 * 3^2 = 1 - 3 + 9 = 7`. The balance is possible.

## Complexity Analysis

-   **Time Complexity**: $O(\log_a B)$
    In each iteration of the `while` loop, the value of `b` is effectively divided by `a`. The number of divisions required to reduce `b` from its initial value (let's call it `B`) to `0` is proportional to $\log_a B$. Since `a >= 2`, the maximum number of iterations occurs when `a=2`, which is $\log_2 B$. Given `B <= 10^9`, $\log_2 10^9 \approx 30$. Each operation inside the loop (modulo, division, addition/subtraction) takes constant time. Thus, the overall time complexity is logarithmic with respect to `b`.

-   **Space Complexity**: $O(1)$
    The algorithm uses a fixed number of variables (`a`, `b`, `remainder`) to store values. No additional data structures are allocated that grow with the input size. Therefore, the space complexity is constant.

## Solution Code

```cpp
class Solution {
public:
    /**
     * @brief Determines if a target weight 'b' can be balanced on a scale using distinct powers of 'a'.
     *
     * The problem asks if the equation `b + (sum of some powers of a) = (sum of some other powers of a)`
     * can be satisfied. This can be rearranged to `b = (sum of some other powers of a) - (sum of some powers of a)`.
     *
     * This implies that 'b' must be representable as a sum `sum(c_i * a^i)`, where each `c_i` can be:
     * - `0`: if `a^i` is not used.
     * - `1`: if `a^i` is placed on the opposite pan (contributing `+a^i` to 'b's side).
     * - `-1`: if `a^i` is placed on the same pan as 'b' (contributing `-a^i` to 'b's side).
     *
     * We can determine the coefficients `c_i` greedily, starting from `c_0` (for `a^0`).
     * Consider the equation `b = c_0 * a^0 + c_1 * a^1 + c_2 * a^2 + ...`
     * Taking this equation modulo `a`, we get `b % a = c_0 % a`.
     * Since `c_0` can only be `-1`, `0`, or `1`:
     *   - If `c_0 = 0`, then `b % a` must be `0`.
     *   - If `c_0 = 1`, then `b % a` must be `1`.
     *   - If `c_0 = -1`, then `b % a` must be `-1 % a`, which is equivalent to `(a-1) % a`.
     *
     * Based on the remainder `b % a`, we make a choice for `c_0` and update `b` for the next iteration:
     * 1. If `b % a == 0`: We choose `c_0 = 0`. The remaining value to balance for higher powers is `b / a`.
     * 2. If `b % a == 1`: We choose `c_0 = 1`. The remaining value to balance is `(b - 1) / a`.
     * 3. If `b % a == a - 1`: We choose `c_0 = -1`. The remaining value to balance is `(b + 1) / a`.
     * 4. If `b % a` is any other value (not `0`, `1`, or `a-1`), it's impossible to satisfy the condition,
     *    so we return `false`.
     *
     * We repeat this process until `b` becomes `0`. If `b` reaches `0`, it means we have successfully
     * represented the original target weight using the allowed coefficients.
     *
     * @param a The base for the powers of weights (2 <= a <= 10^9).
     * @param b The target weight to balance (1 <= b <= 10^9).
     * @return true if 'b' can be balanced, false otherwise.
     */
    bool balancePan(long long a, long long b) {
        // Loop until b becomes 0.
        // In each iteration, 'b' represents the remaining value to be balanced,
        // and we determine the coefficient for the current power of 'a' (a^0, then a^1, etc.).
        while (b > 0) {
            long long remainder = b % a;

            if (remainder == 0) {
                // If remainder is 0, we don't need a^k for this position (coefficient 0).
                // Update b to consider the next higher power: b = b / a.
                b /= a;
            } else if (remainder == 1) {
                // If remainder is 1, we use a^k with coefficient +1.
                // This means b = 1 * a^k + a * (remaining_value).
                // So, (b - 1) must be divisible by 'a'.
                // Update b: b = (b - 1) / a.
                b = (b - 1) / a;
            } else if (remainder == a - 1) {
                // If remainder is a-1, we use a^k with coefficient -1.
                // This means b = -1 * a^k + a * (remaining_value).
                // So, (b + 1) must be divisible by 'a'.
                // Update b: b = (b + 1) / a.
                b = (b + 1) / a;
            } else {
                // If the remainder is anything else (e.g., 2 when a=4), it's impossible
                // to choose a coefficient from {-1, 0, 1} to match the remainder.
                return false;
            }
        }

        // If the loop completes, 'b' has been successfully reduced to 0,
        // indicating that the original target weight can be balanced.
        return true;
    }
};
```