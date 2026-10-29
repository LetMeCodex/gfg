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
            long long term_for_prefix = (pow2[cnt[k]] - 1 + MOD) % MOD;
            prefix_prod_must_include = (prefix_prod_must_include * term_for_prefix) % MOD;

            // Update `suffix_prod_can_include` for k+1.
            // It needs to include numbers from k+2 to N. So, we remove the contribution of k+1.
            // We divide by (2^cnt[k+1]).
            // If k+1 > N, cnt[k+1] is 0, pow2[0] is 1, so division by 1 doesn't change `suffix_prod_can_include`, which is correct.
            if (k + 1 <= N) {
                suffix_prod_can_include = (suffix_prod_can_include * modInverse(pow2[cnt[k + 1]])) % MOD;
            }
        }

        return static_cast<int>(total_ans);
    }
};