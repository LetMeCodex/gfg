# [Maximum Without 3 Consecutive Set Bits](https://www.geeksforgeeks.org/problems/geek-hates-too-many-1s--170647/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 4 attempt(s)

## Problem Summary

Given an integer `n`, we need to find the largest possible integer `ans` such that:
1. `ans` is less than or equal to `n`.
2. `ans` does not contain three consecutive set bits (i.e., no `111` pattern in its binary representation).

## Intuition & Approach

The problem asks us to find the largest number less than or equal to `n` that adheres to a specific binary constraint. This strongly suggests a greedy approach, working from the most significant bit (MSB) to the least significant bit (LSB).

We want to construct the largest possible number `ans` that is still less than or equal to `n`. We can iterate through the bits of `n` from MSB to LSB. For each bit position `j`, if the `j`-th bit of `n` is 1, we have a choice: we can either set the `j`-th bit in `ans` to 1 or to 0. To maximize `ans`, we prefer to set it to 1 if possible.

The constraint is that `ans` cannot have three consecutive set bits. This means if we are considering setting the `j`-th bit of `ans` to 1, we must check if the `(j+1)`-th and `(j+2)`-th bits of `ans` are *already* set to 1. If both are set, then setting the `j`-th bit would create `111`, which is forbidden. In this scenario, we *must* set the `j`-th bit of `ans` to 0, even if the `j`-th bit of `n` is 1.

If setting the `j`-th bit to 1 does not violate the three consecutive set bits rule, we should set it to 1 to maximize `ans`. If the `j`-th bit of `n` is 0, we have no choice but to set the `j`-th bit of `ans` to 0, as `ans` cannot exceed `n`.

**Algorithm:**

1. Initialize `ans` to 0.
2. Iterate from the most significant bit (e.g., bit 31 for a 32-bit integer) down to the least significant bit (bit 0).
3. For each bit position `j`:
    a. Check if the `j`-th bit of `n` is set (`(n >> j) & 1`).
    b. If the `j`-th bit of `n` is set:
        i. Check if the `(j+1)`-th bit of `ans` is set (`(ans >> (j + 1)) & 1`).
        ii. Check if the `(j+2)`-th bit of `ans` is set (`(ans >> (j + 2)) & 1`).
        iii. If both `(j+1)`-th and `(j+2)`-th bits of `ans` are set, then setting the `j`-th bit would create `111`. In this case, we *cannot* set the `j`-th bit of `ans` to 1. We leave it as 0.
        iv. Otherwise (if setting the `j`-th bit to 1 does not violate the rule), set the `j`-th bit of `ans` to 1 (`ans |= (1 << j)`).
    c. If the `j`-th bit of `n` is not set, we cannot set the `j`-th bit of `ans` to 1, so it remains 0.

This greedy approach works because by prioritizing setting higher bits to 1 whenever possible, we ensure that `ans` is as large as possible while respecting the constraint. If we are forced to set a bit to 0 due to the constraint, it's the best we can do for that bit position to maintain the maximum possible value.

The provided solution code implements this exact logic. The checks `(j + 1 <= 31)` and `(j + 2 <= 31)` are good practice to prevent potential issues with shifting beyond the integer's bit width, although for standard integer types, shifting by a value greater than or equal to the bit width typically results in 0.

## Complexity Analysis

- **Time Complexity**: $O(\log N)$, where $N$ is the maximum possible value of `n`. Since we iterate through the bits of the integer (typically 32 or 64 bits), the time complexity is constant with respect to the input value `n` itself, but rather depends on the number of bits in the integer type. For a 32-bit integer, it's $O(32)$, which is $O(1)$. If we consider `n` as the input value, and the number of bits is $\log n$, then it's $O(\log n)$.

- **Space Complexity**: $O(1)$. We are only using a few variables to store the result and loop counters, which does not depend on the input size.

## Solution Code

```cpp
class Solution {
public:
    int maxValue(int n) {
        int ans = 0; // Initialize the result to 0

        // Iterate from the most significant bit (31 for a 32-bit integer) down to 0
        for (int j = 31; j >= 0; --j) {
            // Check if the j-th bit of the original number 'n' is set
            if ((n >> j) & 1) {
                // If the j-th bit of 'n' is 1, we consider setting it in 'ans'.
                // We can only set it if it doesn't create three consecutive set bits.

                // Check if the (j+1)-th bit in 'ans' is set
                // (j + 1 <= 31) ensures we don't try to shift beyond integer limits,
                // though for standard int types, shifting by >= bit width results in 0.
                bool prev_bit_is_set = (j + 1 <= 31) && ((ans >> (j + 1)) & 1);

                // Check if the (j+2)-th bit in 'ans' is set
                bool prev_prev_bit_is_set = (j + 2 <= 31) && ((ans >> (j + 2)) & 1);

                // If both the (j+1)-th and (j+2)-th bits in 'ans' are set,
                // then setting the j-th bit would create '111'.
                if (prev_bit_is_set && prev_prev_bit_is_set) {
                    // In this case, we must set the j-th bit to 0.
                    // Since 'ans' is initialized to 0 and we only set bits when allowed,
                    // doing nothing here effectively keeps the j-th bit as 0.
                } else {
                    // Otherwise, it's safe to set the j-th bit to 1.
                    ans |= (1 << j);
                }
            }
            // If the j-th bit of 'n' is 0, we cannot change it to 1 (problem constraint).
            // So, the j-th bit of 'ans' remains 0. No action needed.
        }
        return ans;
    }
};
```