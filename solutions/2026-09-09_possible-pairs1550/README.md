# [Pairs with Given GCD and LCM](https://www.geeksforgeeks.org/problems/possible-pairs1550/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 3 attempt(s)

## Problem Summary

Given two positive integers `x` and `y`, the task is to find the number of ordered pairs `(a, b)` of positive integers such that their greatest common divisor (GCD) is `x` and their least common multiple (LCM) is `y`.
The problem specifies that `(a, b)` and `(b, a)` are considered distinct pairs if `a ≠ b`.

**Constraints**:
- `1 <= x, y <= 10^9`

## Intuition & Approach

The problem asks us to find pairs `(a, b)` satisfying `gcd(a, b) = x` and `lcm(a, b) = y`. We can leverage fundamental properties of GCD and LCM to simplify this problem.

1.  **Initial Check: The Fundamental Property**
    A crucial property of GCD and LCM is that for any two positive integers `a` and `b`, `lcm(a, b)` must always be a multiple of `gcd(a, b)`.
    Therefore, if `y` is not perfectly divisible by `x` (i.e., `y % x != 0`), it's impossible to find any such pairs `(a, b)`. In this case, we can immediately return `0`.

2.  **Relating `a`, `b`, `x`, and `y`**
    Another key property is: `gcd(a, b) * lcm(a, b) = a * b`.
    Substituting the given `x` and `y`:
    `x * y = a * b`

    Since `x` is the GCD of `a` and `b`, we can express `a` and `b` in terms of `x` and some other integers `a'` and `b'`:
    `a = x * a'`
    `b = x * b'`
    Here, `a'` and `b'` must be positive integers, and critically, their GCD must be 1 (i.e., `gcd(a', b') = 1`). This is because if `a'` and `b'` shared a common factor greater than 1, then `x` wouldn't be the *greatest* common divisor of `a` and `b`.

3.  **Simplifying the Problem**
    Now, substitute `a = x * a'` and `b = x * b'` into the product equation `x * y = a * b`:
    `x * y = (x * a') * (x * b')`
    `x * y = x^2 * a' * b'`

    Since `x >= 1` (from constraints), we can safely divide both sides by `x`:
    `y = x * a' * b'`

    Rearranging this equation to isolate `a' * b'`:
    `a' * b' = y / x`

    Let `P = y / x`. Our problem has now been transformed into finding pairs `(a', b')` such that:
    1.  `a' * b' = P`
    2.  `gcd(a', b') = 1`

    For every such valid pair `(a', b')`, we can construct a unique pair `(a, b) = (x * a', x * b')` that satisfies the original conditions.

4.  **Iterating and Counting Valid Pairs `(a', b')`**
    To find pairs `(a', b')` whose product is `P`, we can iterate through possible values for `a'`. Since `a'` must be a divisor of `P`, we only need to check `a'` up to `sqrt(P)`. If `a'` is a divisor of `P`, then `b' = P / a'` will automatically be the corresponding divisor.

    The algorithm proceeds as follows:
    - Initialize `count = 0`.
    - Calculate `P = y / x`.
    - Iterate `a_prime` from `1` up to `sqrt(P)`:
        - If `a_prime` is a divisor of `P` (i.e., `P % a_prime == 0`):
            - Calculate `b_prime = P / a_prime`.
            - Check if `gcd(a_prime, b_prime) == 1`.
            - If they are coprime:
                - If `a_prime == b_prime`: This means `P` is a perfect square, and `a_prime = b_prime = sqrt(P)`. In this case, `a = x * a_prime` and `b = x * b_prime` implies `a = b`. The problem states `(a, b)` and `(b, a)` are distinct only when `a ≠ b`. So, `(a, a)` is counted once. Increment `count` by `1`.
                - If `a_prime != b_prime`: This means we found two distinct factors `a_prime` and `b_prime`. These lead to two distinct original pairs: `(x * a_prime, x * b_prime)` and `(x * b_prime, x * a_prime)`. Increment `count` by `2`.

    Finally, return `count`.

**Example Walkthrough**: `x = 2, y = 12`
1.  `y % x == 0` (12 % 2 == 0) is true.
2.  `P = y / x = 12 / 2 = 6`.
3.  Initialize `count = 0`.
4.  Loop `a_prime` from `1` to `sqrt(6)` (i.e., `1, 2`):
    -   `a_prime = 1`:
        -   `6 % 1 == 0`.
        -   `b_prime = 6 / 1 = 6`.
        -   `std::gcd(1, 6) == 1`. True.
        -   `a_prime != b_prime`. Increment `count` by `2`. `count = 2`.
        -   (Original pairs: `(2*1, 2*6) = (2, 12)` and `(2*6, 2*1) = (12, 2)`)
    -   `a_prime = 2`:
        -   `6 % 2 == 0`.
        -   `b_prime = 6 / 2 = 3`.
        -   `std::gcd(2, 3) == 1`. True.
        -   `a_prime != b_prime`. Increment `count` by `2`. `count = 4`.
        -   (Original pairs: `(2*2, 2*3) = (4, 6)` and `(2*3, 2*2) = (6, 4)`)
5.  Loop ends. Return `count = 4`.

## Complexity Analysis

*   **Time Complexity**:
    - The initial check `y % x != 0` is $O(1)$.
    - The loop iterates `a_prime` from `1` up to `sqrt(P)`, where `P = y / x`. This means the loop runs approximately `sqrt(P)` times.
    - Inside the loop, the `std::gcd(a_prime, b_prime)` operation takes logarithmic time, specifically $O(\log(\min(a_prime, b_prime)))$. Since `a_prime` and `b_prime` can be up to `P`, this is $O(\log P)$.
    - Therefore, the total time complexity is $O(\sqrt{P} \cdot \log P)$.
    - Given `y` can be up to `10^9`, `P` can also be up to `10^9`. `sqrt(10^9)` is approximately `31,622`. `log(10^9)` (base 2) is approximately `30`. So, the operations are roughly `31622 * 30`, which is about `9.5 * 10^5`. This is well within typical time limits for competitive programming (usually `10^8` operations per second).

*   **Space Complexity**:
    - The solution uses a few integer variables (`x`, `y`, `P`, `count`, `a_prime`, `b_prime`) to store intermediate values.
    - This requires a constant amount of extra space, regardless of the input size.
    - Therefore, the space complexity is $O(1)$.

## Solution Code

```cpp
#include <numeric> // Required for std::gcd

class Solution {
public:
    // Function to count pairs (a, b) such that gcd(a, b) = x and lcm(a, b) = y.
    // (a, b) and (b, a) are counted as distinct pairs when a ≠ b.
    // Renamed function from 'countPairs' to 'pairCount' to match the driver code.
    int pairCount(int x, int y) {
        // Step 1: Critical Requirement - LCM must always be a multiple of GCD.
        // If y is not divisible by x, no such pair (a, b) can exist.
        if (y % x != 0) {
            return 0;
        }

        // Step 2: Reduce the problem.
        // We know that for any two positive integers a and b:
        // gcd(a, b) * lcm(a, b) = a * b
        // Given gcd(a, b) = x and lcm(a, b) = y, we have:
        // x * y = a * b

        // Also, since x is the GCD of a and b, we can write:
        // a = x * a'
        // b = x * b'
        // where a' and b' are positive integers and gcd(a', b') = 1.

        // Substitute these into the product equation:
        // x * y = (x * a') * (x * b')
        // x * y = x^2 * a' * b'

        // Divide by x (since x >= 1 by constraints, it's safe):
        // y = x * a' * b'
        // a' * b' = y / x

        // Let P = y / x. Our problem reduces to finding pairs (a', b') such that:
        // 1. a' * b' = P
        // 2. gcd(a', b') = 1
        // For each such pair (a', b'), we can form the original pair (a, b) = (x * a', x * b').

        int P = y / x;
        int count = 0;

        // Step 3: Iterate through possible values for a' and count valid pairs.
        // a' must be a divisor of P. We only need to check a' up to sqrt(P)
        // because if a' is a divisor, then b' = P / a' is also a divisor.
        for (int a_prime = 1; a_prime * a_prime <= P; ++a_prime) {
            // Check if a_prime is a divisor of P
            if (P % a_prime == 0) {
                int b_prime = P / a_prime;

                // Check the second condition: gcd(a', b') = 1
                if (std::gcd(a_prime, b_prime) == 1) {
                    // If a_prime == b_prime, it means P is a perfect square and a_prime = sqrt(P).
                    // In this case, (a, b) = (x * a_prime, x * a_prime) implies a = b.
                    // The problem states (a, b) and (b, a) are distinct only when a ≠ b.
                    // So, if a = b, it's counted once.
                    if (a_prime == b_prime) {
                        count += 1;
                    } else {
                        // If a_prime != b_prime, then (x * a_prime, x * b_prime) and
                        // (x * b_prime, x * a_prime) are two distinct pairs.
                        count += 2;
                    }
                }
            }
        }

        return count;
    }
};
```