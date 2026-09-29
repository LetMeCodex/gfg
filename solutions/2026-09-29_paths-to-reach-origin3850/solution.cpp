#include <vector>

class Solution {
public:
    static const int MOD = 1000000007;
    static const int MAX_COORD_SUM = 1000; // Max x+y = 500+500

    // Modular exponentiation: calculates (base^exp) % MOD
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