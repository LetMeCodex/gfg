# GeeksforGeeks POTD: Ways to Reach Origin

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find the number of distinct paths from the origin (0,0) to a given point (x,y) on a 2D grid. The only allowed moves are one step to the right (increasing x by 1) or one step down (increasing y by 1).

## Intuition & Approach

This problem is a classic combinatorial problem. To reach the point (x,y) from (0,0) by only moving right or down, we must make a total of `x` moves to the right and `y` moves down. The total number of moves will be `x + y`.

The problem then boils down to finding the number of ways to arrange these `x` 'Right' moves and `y` 'Down' moves in a sequence of `x + y` total moves. This is equivalent to choosing `x` positions for the 'Right' moves (or `y` positions for the 'Down' moves) out of a total of `x + y` positions.

This can be solved using the binomial coefficient formula:
$$C(n, k) = \binom{n}{k} = \frac{n!}{k!(n-k)!}$$

In our case, `n` is the total number of moves, which is `x + y`, and `k` can be either `x` (number of right moves) or `y` (number of down moves). So, the number of ways is:
$$C(x+y, x) = \frac{(x+y)!}{x!(y)!}$$

Since the problem involves a large number of paths, the result can exceed the standard integer limits. Therefore, we need to compute the result modulo a large prime number, which is given as $10^9 + 7$ (MOD).

To compute the binomial coefficient modulo a prime number, we can use the formula:
$$C(n, k) \pmod{MOD} = (n! \times (k!)^{-1} \times ((n-k)! )^{-1}) \pmod{MOD}$$

Here, $(k!)^{-1}$ and $((n-k)! )^{-1}$ represent the modular multiplicative inverses of $k!$ and $(n-k)!$ respectively, modulo MOD. Since MOD ($10^9 + 7$) is a prime number, we can use Fermat's Little Theorem to find the modular inverse:
$$a^{MOD-2} \equiv a^{-1} \pmod{MOD}$$

So, $(k!)^{-1} \equiv (k!)^{MOD-2} \pmod{MOD}$ and $((n-k)! )^{-1} \equiv ((n-k)! )^{MOD-2} \pmod{MOD}$.

The overall approach involves:
1.  **Precomputing Factorials**: To efficiently calculate $n!$, $k!$, and $(n-k)!$, we can precompute factorials up to the maximum possible sum of `x` and `y` (which is $500 + 500 = 1000$). This precomputation should be done only once.
2.  **Modular Exponentiation**: A function to compute `(base^exp) % MOD` efficiently. This is needed for calculating modular inverses.
3.  **Modular Inverse**: A function to compute the modular inverse using modular exponentiation based on Fermat's Little Theorem.
4.  **Calculating Combinations**: Using the precomputed factorials and modular inverses, calculate $C(x+y, x) \pmod{MOD}$.

The provided solution implements these steps. The `getFactorials()` function uses a static vector and a flag to ensure factorials are computed only once. The `power()` function implements modular exponentiation, and `modInverse()` uses `power()` to find the modular inverse. Finally, the `ways()` function orchestrates these components to compute the result.

The two attempts were likely due to:
*   **Attempt 1**: Potentially an issue with handling modulo operations at each step, leading to overflow before the modulo is applied. Or, an incorrect implementation of modular inverse or exponentiation.
*   **Attempt 2**: A corrected implementation addressing the issues from the first attempt, ensuring all intermediate calculations are performed modulo MOD.

## Complexity Analysis

-   **Time Complexity**:
    -   **Precomputation of Factorials**: $O(\text{MAX\_COORD\_SUM})$ where MAX\_COORD\_SUM is the maximum possible value of `x + y`. In this problem, it's $500 + 500 = 1000$. This is done only once.
    -   **Modular Exponentiation**: $O(\log MOD)$ for each modular inverse calculation.
    -   **`ways()` function**: The dominant operations are modular inverse calculations. We perform two modular inverse calculations. Thus, the time complexity for each call to `ways()` is $O(\log MOD)$.
    -   **Overall**: Since precomputation is done once, and subsequent calls to `ways()` are efficient, the amortized time complexity per test case is $O(\log MOD)$. If we consider the initial precomputation, it's $O(\text{MAX\_COORD\_SUM} + \log MOD)$.

-   **Space Complexity**:
    -   **Factorials**: $O(\text{MAX\_COORD\_SUM})$ to store the precomputed factorials.
    -   **Other variables**: $O(1)$.
    -   **Overall**: $O(\text{MAX\_COORD\_SUM})$.

## Solution Code

```cpp
#include <vector>

class Solution {
public:
    static const int MOD = 1000000007;
    static const int MAX_COORD_SUM = 1000; // Max x+y = 500+500

    // Modular exponentiation: calculates (base^exp) % MOD
    // Uses binary exponentiation for efficiency.
    long long power(long long base, long long exp) {
        long long res = 1;
        base %= MOD; // Ensure base is within modulo range
        while (exp > 0) {
            if (exp % 2 == 1) { // If exp is odd, multiply result by base
                res = (res * base) % MOD;
            }
            base = (base * base) % MOD; // Square the base
            exp /= 2; // Halve the exponent
        }
        return res;
    }

    // Modular inverse using Fermat's Little Theorem: calculates (n^(MOD-2)) % MOD
    // This works because MOD is a prime number.
    long long modInverse(long long n) {
        return power(n, MOD - 2);
    }

    // Function to get precomputed factorials.
    // Uses a local static vector and a static boolean flag to ensure
    // factorials are computed only once across all calls and test cases.
    const std::vector<long long>& getFactorials() {
        static std::vector<long long> fact_vec;
        static bool factorials_precomputed = false;

        if (!factorials_precomputed) {
            fact_vec.resize(MAX_COORD_SUM + 1);
            fact_vec[0] = 1; // 0! = 1
            for (int i = 1; i <= MAX_COORD_SUM; i++) {
                fact_vec[i] = (fact_vec[i - 1] * i) % MOD;
            }
            factorials_precomputed = true;
        }
        return fact_vec;
    }

    // Main function to find the number of ways to reach origin
    // The method name was changed from 'waysToReachOrigin' to 'ways' to match the driver code.
    int ways(int x, int y) {
        // Retrieve the precomputed factorials.
        // This call will trigger computation only on the first invocation across all test cases.
        const std::vector<long long>& fact = getFactorials();

        // Total number of moves N = x (left moves) + y (down moves)
        int N = x + y;
        // Number of 'left' moves (or 'down' moves, it's symmetric for combinations)
        int K = x; 

        // Handle edge cases where x or y is 0.
        // If x=0, all moves must be down. There's only 1 way.
        // If y=0, all moves must be left. There's only 1 way.
        // This is implicitly handled by the combination formula C(N, 0) = 1 and C(N, N) = 1.
        // However, if N=0 (i.e., x=0, y=0), C(0,0) = 1, which is correct.

        // Calculate C(N, K) = N! / (K! * (N-K)!) mod MOD
        // This is equivalent to (N! * (K!)^(-1) * ((N-K)!)^(-1)) mod MOD
        
        long long numerator = fact[N];
        long long denominator_k = fact[K];
        long long denominator_nk = fact[N - K];

        // Calculate modular inverses for the denominators
        long long inv_denominator_k = modInverse(denominator_k);
        long long inv_denominator_nk = modInverse(denominator_nk);

        // Combine the terms, taking modulo at each multiplication to prevent overflow
        long long ans = (numerator * inv_denominator_k) % MOD;
        ans = (ans * inv_denominator_nk) % MOD;

        return (int)ans; // Cast to int as the problem expects int return type
    }
};
```