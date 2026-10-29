# GeeksforGeeks POTD: Subsets with MEX in Range

## Problem Link
[Subsets with MEX in Range](https://www.geeksforgeeks.org/problems/crazy-subsets--131708/1)

## Difficulty
Hard

## Solved In
4 attempt(s)

## Problem Summary

Given an array `arr` of size `N` and two integers `l` and `r`, we need to find the total number of subsets of `arr` such that their Minimum Excluded (MEX) value is within the range `[l, r]`. The MEX of a set is the smallest non-negative integer that is not present in the set. The result should be returned modulo $10^9 + 7$.

## Intuition & Approach

The core of this problem lies in understanding the definition of MEX and how it relates to subsets. A subset has a MEX of `k` if and only if:
1. All non-negative integers from `0` to `k-1` are present in the subset.
2. The integer `k` is *not* present in the subset.
3. Any integers greater than `k` can be present or absent.

Since we are dealing with subsets and counting, dynamic programming or combinatorial approaches are usually effective. The constraints on the input array size `N` (up to $10^5$) suggest that an $O(N^2)$ or higher complexity solution might be too slow. We need something closer to $O(N)$ or $O(N \log N)$.

The problem asks for the sum of counts of subsets with MEX in the range `[l, r]`. This can be calculated as:
$\sum_{k=l}^{r} (\text{Number of subsets with MEX = } k)$

Let's focus on calculating the number of subsets with MEX exactly equal to `k`.
For a subset to have MEX = `k`:
*   It must contain at least one instance of each number from `0` to `k-1`.
*   It must *not* contain the number `k`.
*   It can contain any combination of numbers from `k+1` up to `N` (and beyond, but numbers greater than `N` don't affect MEX values up to `N`).

Consider the counts of each number in the original array `arr`. Let `cnt[x]` be the number of times `x` appears in `arr`.

To ensure that a number `i` (where `0 <= i < k`) is present in a subset, we must pick at least one occurrence of `i`. If `cnt[i]` is the frequency of `i`, the total number of ways to pick at least one `i` is `(2^cnt[i] - 1)` (total subsets of `i` minus the empty subset).

To ensure that the number `k` is *not* present in a subset, we simply don't pick any occurrences of `k`.

For numbers `j` where `j > k`, we can pick any subset of their occurrences. If `cnt[j]` is the frequency of `j`, the number of ways to pick occurrences of `j` is `2^cnt[j]`.

Therefore, the number of subsets with MEX = `k` can be calculated as:
$(\prod_{i=0}^{k-1} (2^{cnt[i]} - 1)) \times (\prod_{j=k+1}^{N} 2^{cnt[j]}) \pmod{MOD}$

The numbers greater than `N` in the input array do not affect the MEX if the MEX is less than or equal to `N`. We only need to consider numbers up to `N` for calculating MEX values up to `N`.

A naive approach would be to iterate `k` from `l` to `r` and for each `k`, calculate the two products. This would involve iterating through the array for each `k`, leading to an $O((r-l+1) \cdot N)$ complexity, which is too slow if `r` is close to `N`.

We can optimize this by precomputing frequencies and using prefix/suffix products.

1.  **Frequency Count**: First, count the occurrences of each number in `arr`. We only care about numbers up to `N`, as any number greater than `N` will not affect the MEX if it's less than or equal to `N`.
2.  **Powers of 2**: Precompute powers of 2 modulo $10^9 + 7$ up to `N`. `pow2[i] = 2^i % MOD`.
3.  **Iterative Calculation**: We can iterate through possible MEX values `k` from `0` up to `r`. For each `k`, we need to efficiently calculate the number of subsets with MEX = `k`.

    Let's maintain two running products:
    *   `prefix_prod_must_include`: This will store $\prod_{i=0}^{k-1} (2^{cnt[i]} - 1) \pmod{MOD}$.
    *   `suffix_prod_can_include`: This will store $\prod_{j=k+1}^{N} 2^{cnt[j]} \pmod{MOD}$.

    We can iterate `k` from `0` to `r`.
    *   **Initialization**:
        *   `prefix_prod_must_include` starts at 1 (for `k=0`, the product from `0` to `-1` is an empty product, which is 1).
        *   `suffix_prod_can_include` can be initialized for `k=0`. This means it will store $\prod_{j=1}^{N} 2^{cnt[j]} \pmod{MOD}$.
    *   **For each `k`**:
        *   The number of subsets with MEX = `k` is `(prefix_prod_must_include * suffix_prod_can_include) % MOD`.
        *   If `k >= l`, add this count to the `total_ans`.
        *   **Update for `k+1`**:
            *   To get the `prefix_prod_must_include` for `k+1`, we need to include the term for `k`. So, multiply the current `prefix_prod_must_include` by `(2^cnt[k] - 1) % MOD`. If `cnt[k]` is 0, this term becomes `(2^0 - 1) = 0`, correctly making the prefix product zero if `k` is missing, implying no subset can have MEX greater than `k`.
            *   To get the `suffix_prod_can_include` for `k+1`, we need to exclude the term for `k+1`. So, divide the current `suffix_prod_can_include` by `2^cnt[k+1] % MOD`. This is equivalent to multiplying by the modular inverse of `2^cnt[k+1]`. If `k+1 > N`, `cnt[k+1]` is 0, `2^0 = 1`, so division by 1 doesn't change the product, which is correct.

    The modular inverse is needed for division. We can use Fermat's Little Theorem for modular inverse since the modulus ($10^9 + 7$) is a prime number. $a^{p-2} \equiv a^{-1} \pmod{p}$.

The loop for `k` runs from `0` to `r`. Inside the loop, operations are $O(1)$ after precomputation. The precomputation of frequencies takes $O(N)$. Precomputation of powers of 2 takes $O(N)$. The modular inverse calculation takes $O(\log MOD)$ using modular exponentiation. The overall time complexity will be dominated by the frequency counting and the loop up to `r`. Since `r` can be up to `N`, the complexity is $O(N + \log MOD)$.

**Edge Cases and Considerations:**
*   **`cnt[k] == 0`**: If `cnt[k]` is 0, then the number `k` is not present in the original array.
    *   If we are calculating subsets with MEX = `k`, this condition is met (number `k` is not present).
    *   If we are calculating subsets with MEX > `k`, then `k` must be present. Since `cnt[k]` is 0, it's impossible to include `k`. This means no subset can have MEX greater than `k`. Our `prefix_prod_must_include` calculation handles this: `(2^0 - 1) = 0`, making the product zero.
*   **Modular Arithmetic**: All intermediate calculations must be done modulo $10^9 + 7$. The `(a - b + MOD) % MOD` pattern is crucial for subtractions.

The problem statement implies the function should be named `subsetsWithMEX`. However, the driver code in the diagnostic shows it's expecting `cntSubsets`. The provided solution renames the function to `cntSubsets` to match the driver code's expectation.

```cpp
#include <vector>
#include <numeric>

class Solution {
public:
    // Modular exponentiation: (base^exp) % MOD
    long long power(long long base, long long exp) {
        long long res = 1;
        long long MOD = 1000000007;
        base %= MOD;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

    // Modular inverse using Fermat's Little Theorem: n^(MOD-2) % MOD
    long long modInverse(long long n) {
        long long MOD = 1000000007;
        return power(n, MOD - 2);
    }

    // The problem statement implies the function should be named subsetsWithMEX.
    // The driver code in the diagnostic shows it's expecting cntSubsets.
    // Renaming the function to match the driver code's expectation.
    int cntSubsets(std::vector<int>& arr, int l, int r) {
        int N = arr.size();
        long long MOD = 1000000007;

        // Step 1: Count frequencies of each number in arr
        // Numbers greater than N do not affect MEX values up to N.
        // We only care about numbers from 0 to N.
        std::vector<int> cnt(N + 1, 0);
        for (int x : arr) {
            if (x <= N) {
                cnt[x]++;
            }
        }

        // Step 2: Precompute powers of 2 modulo MOD
        std::vector<long long> pow2(N + 1);
        pow2[0] = 1;
        for (int i = 1; i <= N; ++i) {
            pow2[i] = (pow2[i - 1] * 2) % MOD;
        }

        // Step 3: Calculate the number of subsets with MEX exactly k for k in [l, r]
        // A subset has MEX = k if and only if:
        // 1. It contains all numbers from 0 to k-1.
        // 2. It does NOT contain the number k.
        // 3. It can contain any combination of numbers from k+1 up to N.

        // Let's define:
        // `ways_to_include_0_to_k_minus_1`: Number of ways to form a subset that *must* contain at least one of each number from 0 to k-1.
        // For each number `i` from 0 to k-1, we have `cnt[i]` occurrences. To ensure `i` is present, we must pick at least one occurrence. The number of ways to do this is `(2^cnt[i] - 1)`.
        // So, `ways_to_include_0_to_k_minus_1` = product_{i=0}^{k-1} (2^cnt[i] - 1) mod MOD.

        // `ways_to_include_k_plus_1_to_N`: Number of ways to form a subset using numbers from k+1 to N.
        // For each number `j` from k+1 to N, we have `cnt[j]` occurrences. We can pick any subset of these occurrences. The number of ways is `2^cnt[j]`.
        // So, `ways_to_include_k_plus_1_to_N` = product_{j=k+1}^{N} (2^cnt[j]) mod MOD.

        // The number of subsets with MEX = k is `ways_to_include_0_to_k_minus_1` * `ways_to_include_k_plus_1_to_N` mod MOD.
        // The number `k` itself is excluded from both products, ensuring it's not present in the subset.

        // We can compute these products iteratively.
        // Let `prefix_prod_must_include` store the product of (2^cnt[i] - 1) for i from 0 to k-1.
        // Let `suffix_prod_can_include` store the product of (2^cnt[j]) for j from k+1 to N.

        long long total_ans = 0;
        long long prefix_prod_must_include = 1; // Represents product for 0 to k-1
        long long suffix_prod_can_include = 1;  // Represents product for k+1 to N

        // Initialize `suffix_prod_can_include` for k=0. This is the product for numbers 1 to N.
        // For k=0, MEX=0 means 0 is not in the subset.
        // The numbers from 1 to N can be included in any way.
        // So, suffix_prod_can_include should be product of 2^cnt[j] for j from 1 to N.
        for (int j = 1; j <= N; ++j) {
            suffix_prod_can_include = (suffix_prod_can_include * pow2[cnt[j]]) % MOD;
        }

        // Iterate k from 0 up to r. k represents the target MEX value.
        for (int k = 0; k <= r; ++k) {
            // For MEX = k:
            // We need to include all numbers from 0 to k-1. The ways are `prefix_prod_must_include`.
            // We must NOT include k.
            // We can include any numbers from k+1 to N. The ways are `suffix_prod_can_include`.

            // The number of subsets with MEX exactly k is `prefix_prod_must_include` * `suffix_prod_can_include` mod MOD.
            long long count_mex_k = (prefix_prod_must_include * suffix_prod_can_include) % MOD;

            // If the target MEX k is within the range [l, r], add it to the total answer.
            if (k >= l) {
                total_ans = (total_ans + count_mex_k) % MOD;
            }

            // Prepare for the next iteration (k+1).

            // Update `prefix_prod_must_include` for k+1.
            // It needs to include numbers from 0 to k. So, we multiply by (2^cnt[k] - 1).
            // If cnt[k] is 0, (2^0 - 1) = 0. This correctly makes `prefix_prod_must_include` zero if number k is missing, meaning no subset can have MEX > k.
            long long term_for_prefix = (pow2[cnt[k]] - 1 + MOD) % MOD; // (2^cnt[k] - 1) mod MOD
            prefix_prod_must_include = (prefix_prod_must_include * term_for_prefix) % MOD;

            // Update `suffix_prod_can_include` for k+1.
            // It needs to include numbers from k+2 to N. So, we remove the contribution of k+1.
            // We divide by (2^cnt[k+1]).
            // If k+1 > N, cnt[k+1] is 0, pow2[0] is 1, so division by 1 doesn't change `suffix_prod_can_include`, which is correct.
            if (k + 1 <= N) {
                // We need to divide by pow2[cnt[k+1]].
                // This is equivalent to multiplying by the modular inverse of pow2[cnt[k+1]].
                suffix_prod_can_include = (suffix_prod_can_include * modInverse(pow2[cnt[k + 1]])) % MOD;
            }
        }

        return static_cast<int>(total_ans);
    }
};
```

## Complexity Analysis

*   **Time Complexity**: $O(N + \log MOD)$
    *   Counting frequencies: $O(N)$
    *   Precomputing powers of 2: $O(N)$
    *   Initializing `suffix_prod_can_include`: $O(N)$
    *   The main loop iterates from `k = 0` to `r`. Since `r` can be up to `N`, this loop runs at most $N+1$ times.
    *   Inside the loop, operations like modular multiplication, addition, and modular inverse calculation (which uses modular exponentiation) take $O(1)$ and $O(\log MOD)$ respectively. The modular inverse is called once per iteration for `suffix_prod_can_include` update.
    *   Therefore, the total time complexity is $O(N + N \cdot \log MOD)$ if we consider the modular inverse call in each iteration. However, the modular inverse is only called when `k+1 <= N`. The dominant part is $O(N)$ for precomputation and the loop. The modular inverse calculation is $O(\log MOD)$, and it's called at most $N$ times. So, it's $O(N + N \log MOD)$.
    *   A more precise analysis: The loop runs $r+1$ times. Inside the loop, we do constant time operations and one modular inverse. The modular inverse takes $O(\log MOD)$. So, the loop part is $O(r \log MOD)$. The total is $O(N + r \log MOD)$. Since $r \le N$, this is $O(N \log MOD)$.
    *   **Correction**: The modular inverse is called at most $N$ times. The loop runs $r+1$ times. The total time is $O(N + (r+1) \log MOD)$. Since $r \le N$, it's $O(N \log MOD)$.
    *   **Further Optimization/Clarification**: The modular inverse is called for `pow2[cnt[k+1]]`. The `cnt` values are at most `N`. The `power` function takes $O(\log exp)$. So, `modInverse` takes $O(\log MOD)$. The loop runs $r+1$ times. The total time is $O(N + (r+1) \log MOD)$. Since $r \le N$, it's $O(N \log MOD)$.
    *   **Final Check**: The precomputation of `pow2` is $O(N)$. The frequency count is $O(N)$. The initialization of `suffix_prod_can_include` is $O(N)$. The loop runs $r+1$ times. Inside the loop, we have constant time arithmetic operations and one call to `modInverse`. `modInverse` takes $O(\log MOD)$. So, the loop takes $O(r \log MOD)$. Total time: $O(N + r \log MOD)$. Since $r \le N$, the complexity is $O(N \log MOD)$.

*   **Space Complexity**: $O(N)$
    *   `cnt` vector: $O(N)$
    *   `pow2` vector: $O(N)$
    *   Other variables take $O(1)$ space.
    *   Therefore, the total space complexity is $O(N)$.