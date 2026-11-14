#include <string> // Required for std::string

class Solution {
public:
    // Modulo constant
    const long long MOD = 1e9 + 7;

    /**
     * @brief Calculates (base^exp) % MOD using modular exponentiation (binary exponentiation).
     * @param base The base of the exponentiation.
     * @param exp The exponent.
     * @return (base^exp) % MOD.
     */
    long long power(long long base, long long exp) {
        long long res = 1;
        base %= MOD; // Ensure base is within modulo range
        while (exp > 0) {
            if (exp % 2 == 1) { // If exp is odd, multiply res by base
                res = (res * base) % MOD;
            }
            base = (base * base) % MOD; // Square the base
            exp /= 2; // Halve the exponent
        }
        return res;
    }

    /**
     * @brief Finds the sum of all digits across all non-empty subsequences of s.
     * @param s The input string consisting of digits.
     * @return The total sum modulo 10^9 + 7.
     */
    int subsequenceSum(std::string s) { // Renamed function to subsequenceSum
        int n = s.length();

        // Edge case: if string is empty, sum is 0.
        // Although problem constraints usually imply non-empty string,
        // N=0 would result in N-1 = -1 for power, which is not handled.
        // For N=0, there are no subsequences, so sum is 0.
        if (n == 0) {
            return 0;
        }

        // Step 1: Calculate the sum of all individual digits in the string s.
        // This sum is kept modulo MOD to prevent overflow if N is very large,
        // though for N=10^5 and digits up to 9, 9*10^5 fits in a long long.
        long long total_digit_sum = 0;
        for (char c : s) {
            total_digit_sum = (total_digit_sum + (c - '0')) % MOD;
        }

        // Step 2: Calculate the number of times each digit appears in all non-empty subsequences.
        // As derived, each digit s[i] appears in 2^(N-1) subsequences.
        // We need to calculate 2^(N-1) modulo MOD.
        // For N=1, N-1=0, so 2^0=1. The power function handles this correctly.
        long long num_occurrences_per_digit = power(2, n - 1);

        // Step 3: The final sum is the product of (sum of all digits in s) and
        // (number of times each digit appears), all modulo MOD.
        long long final_sum = (total_digit_sum * num_occurrences_per_digit) % MOD;

        return static_cast<int>(final_sum);
    }
};