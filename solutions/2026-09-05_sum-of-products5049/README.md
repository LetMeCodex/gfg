# [Sum of Pairwise ANDs](https://www.geeksforgeeks.org/problems/sum-of-products5049/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary
Given an array `arr` of `N` integers, the task is to find the sum of `(arr[i] AND arr[j])` for all unique pairs `(i, j)` such that `i < j`.

**Constraints:**
- `1 <= N <= 10^5`
- `1 <= arr[i] <= 10^8`

## Intuition & Approach
A straightforward approach would be to iterate through all possible pairs `(i, j)` where `i < j`, calculate `arr[i] & arr[j]`, and add it to a running total. This would involve $O(N^2)$ operations. Given `N` can be up to $10^5$, an $O(N^2)$ solution would be too slow ($10^{10}$ operations).

We need a more efficient approach. The key insight lies in bit manipulation. Instead of summing the ANDs directly, we can leverage the property that bitwise operations can be analyzed bit by bit. The total sum can be thought of as the sum of contributions from each individual bit position.

Let's consider a specific bit position `b` (e.g., the 0th bit, 1st bit, ..., up to the 30th bit, since $10^8 < 2^{27}$).
1.  **Contribution of a bit:** If the `b`-th bit is set in the result of `(arr[i] AND arr[j])`, it implies that both `arr[i]` and `arr[j]` must have their `b`-th bit set. When this condition is met for a pair, this specific bit contributes `2^b` to the total sum for that pair.

2.  **Counting pairs for each bit:** To find the total contribution of the `b`-th bit to the overall sum, we need to count how many pairs `(i, j)` (with `i < j`) exist such that both `arr[i]` and `arr[j]` have their `b`-th bit set.
    *   We can iterate through the entire array `arr` once for each bit position `b`.
    *   During this iteration, we count how many numbers in `arr` have their `b`-th bit set. Let this count be `k`.
    *   If `k` numbers have the `b`-th bit set, then any pair chosen from these `k` numbers will have their `b`-th bit set in their bitwise AND result.
    *   The number of ways to choose 2 distinct numbers from `k` numbers is given by the combination formula $C(k, 2) = \frac{k \times (k - 1)}{2}$. This value represents the number of pairs that will contribute `2^b` to the total sum.

3.  **Accumulating total sum:**
    *   We iterate through each bit position `b` from 0 up to 30 (since `arr[i] <= 10^8`, the maximum relevant bit is 26 or 27, so 30 is a safe upper bound).
    *   For each bit position `b`:
        *   Calculate `k`, the count of numbers in `arr` with the `b`-th bit set.
        *   If `k > 1` (meaning at least two numbers have the bit set, allowing for at least one pair), calculate `pairs = (k * (k - 1)) / 2`.
        *   Add `pairs * (1LL << b)` to the `totalSum`. The `1LL` is crucial here to ensure that `2^b` is computed as a `long long` before multiplication, preventing potential overflow if `2^b` itself is large and `pairs` is also large.

By summing these contributions for all relevant bit positions, we obtain the final answer. This approach transforms the $O(N^2)$ problem into an $O(N \cdot \text{max_bits})$ problem, which is efficient enough.

## Complexity Analysis
-   **Time Complexity**: $O(N \cdot \text{max_bits})$
    *   The outer loop iterates `max_bits` times (approximately 31 for integers up to $10^8$).
    *   The inner loop iterates `N` times for each bit position to count set bits.
    *   Thus, the total time complexity is proportional to `N` multiplied by the number of bits in the maximum possible integer. Since `max_bits` is a constant (e.g., 31 for a standard 32-bit integer), this can be simplified to $O(N)$.
-   **Space Complexity**: $O(1)$
    *   The algorithm uses a few constant-size variables (`totalSum`, `countSetBits`, `bit`, etc.) regardless of the input array size `N`. No auxiliary data structures are used that scale with `N`.

## Solution Code

```cpp
#include <vector> // Required for std::vector

class Solution {
public:
    /**
     * Calculates the sum of bitwise AND for all pairs (i, j) where i < j.
     * Logic: For each bit position 'b', if 'k' numbers have the 'b'-th bit set,
     * then there are k * (k - 1) / 2 pairs that will have the 'b'-th bit set in their AND result.
     * Each such pair contributes 2^b to the total sum.
     */
    long long pairAndSum(std::vector<int>& arr) {
        long long totalSum = 0;
        int n = arr.size();

        // Iterate through each bit position (0 to 30, since arr[i] <= 10^8 < 2^30)
        // A 32-bit integer can hold values up to 2^31 - 1.
        // 10^8 is less than 2^27, so checking up to bit 30 is sufficient and safe.
        for (int bit = 0; bit < 31; ++bit) {
            long long countSetBits = 0;
            
            // Count how many numbers in the array have the current bit set
            for (int x : arr) {
                // Check if the 'bit'-th bit of 'x' is set
                if ((x >> bit) & 1) {
                    countSetBits++;
                }
            }

            // If 'k' numbers have the 'bit'-th bit set, then there are k * (k - 1) / 2 pairs
            // where both numbers have the 'bit'-th bit set.
            // For each such pair, the 'bit'-th bit contributes 2^bit to the sum of ANDs.
            // We need at least 2 numbers to form a pair.
            if (countSetBits > 1) { 
                long long pairs = (countSetBits * (countSetBits - 1)) / 2;
                // Add the contribution of this bit position to the total sum.
                // (1LL << bit) calculates 2^bit as a long long to prevent overflow
                // before multiplication with 'pairs'.
                totalSum += pairs * (1LL << bit); 
            }
        }

        return totalSum;
    }
};
```