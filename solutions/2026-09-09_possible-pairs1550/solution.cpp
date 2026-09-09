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