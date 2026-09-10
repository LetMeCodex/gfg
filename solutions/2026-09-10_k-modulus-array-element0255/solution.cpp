#include <vector>    // Required for std::vector
#include <numeric>   // Required for std::gcd (available in C++17)
#include <cmath>     // Required for std::abs and std::sqrt

class Solution {
public:
    // Renamed the method from 'countValues' to 'sameMod' as per the GFG compilation error.
    int sameMod(std::vector<int>& arr) {
        // According to constraints, arr.size() >= 1, so arr will not be empty.
        // If arr.size() == 1 (e.g., arr = [10]), there's only one element.
        // Any positive integer k will make 10 % k the remainder for all elements
        // (since there's only one). This means infinitely many k.
        // My current logic handles this:
        // If arr.size() == 1, the loop for differences won't execute.
        // `common_divisor_candidate` will remain 0.
        // The `if (common_divisor_candidate == 0)` block will then execute, returning -1.
        // This is the correct behavior for arr.size() == 1.

        // Step 1: Calculate the greatest common divisor (GCD) of all absolute differences
        // |arr[i] - arr[0]| for i from 1 to arr.size() - 1.
        // If all elements arr[i] leave the same remainder 'r' when divided by 'k',
        // then arr[i] = q_i * k + r for some integer q_i.
        // This implies that arr[i] - arr[j] must be a multiple of k for any i, j.
        // Specifically, k must divide arr[i] - arr[0] for all i > 0.
        // Thus, k must be a common divisor of all |arr[i] - arr[0]|.
        // The values of k that satisfy the condition are precisely the positive divisors
        // of G = gcd(|arr[1]-arr[0]|, |arr[2]-arr[0]|, ..., |arr[N-1]-arr[0]|).

        // We initialize common_divisor_candidate to 0.
        // std::gcd(0, x) = x, std::gcd(x, 0) = x, std::gcd(0, 0) = 0.
        // This property correctly handles the first non-zero difference encountered
        // (it becomes the initial GCD) and subsequent differences.
        int common_divisor_candidate = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            // std::abs is used to ensure positive differences.
            // The differences can be up to 10^5 - 1, which fits within an int.
            common_divisor_candidate = std::gcd(common_divisor_candidate, std::abs(arr[i] - arr[0]));
        }

        // Step 2: Check if common_divisor_candidate is 0.
        // If common_divisor_candidate is 0, it means all absolute differences |arr[i] - arr[0]| were 0.
        // This implies that all elements in the array `arr` are equal to `arr[0]`.
        // If all elements are equal (e.g., [5, 5, 5]), then for any positive integer k,
        // all elements will leave the same remainder (arr[0] % k).
        // In this scenario, there are infinitely many such values of k.
        if (common_divisor_candidate == 0) {
            return -1;
        }

        // Step 3: If common_divisor_candidate is positive, count its positive divisors.
        // The values of k that satisfy the condition are precisely the positive divisors
        // of `common_divisor_candidate`.
        int count = 0;
        // Iterate from 1 up to the square root of common_divisor_candidate.
        // This is an efficient way to find all divisors.
        // The maximum value of common_divisor_candidate is 10^5 - 1, so sqrt(10^5) is approx 316.
        // The loop runs a maximum of ~316 times.
        for (int i = 1; i * i <= common_divisor_candidate; ++i) {
            if (common_divisor_candidate % i == 0) {
                // If 'i' divides common_divisor_candidate, then 'i' is a divisor.
                count++;
                // Also, 'common_divisor_candidate / i' is a divisor.
                // We need to check if 'i' and 'common_divisor_candidate / i' are distinct.
                // They are distinct if i*i is not equal to common_divisor_candidate.
                if (i * i != common_divisor_candidate) {
                    count++;
                }
            }
        }

        return count;
    }
};