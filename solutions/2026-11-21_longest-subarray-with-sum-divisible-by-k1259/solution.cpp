#include <vector>
#include <numeric> // Not strictly needed for the final solution, but good to keep if it was part of development
#include <algorithm> // Required for std::max

class Solution {
public:
    // Renamed the function from longestSubarraySumDivisibleByK to longestSubarrayDivK
    // to match the name expected by the GeeksforGeeks driver code.
    int longestSubarrayDivK(std::vector<int>& arr, int k) {
        int n = arr.size();
        
        // Use a vector to store the first occurrence of each remainder.
        // The size of the vector is k, as remainders will be in [0, k-1].
        // Initialize with -2 to indicate that the remainder has not been seen yet.
        // -1 is reserved for the initial state of remainder 0.
        std::vector<int> first_occurrence(k, -2); 
        
        // Initialize max_len to 0.
        int max_len = 0;
        
        // current_sum can accumulate up to 10^6 * 10^6 = 10^12,
        // so it must be a long long to prevent overflow.
        long long current_sum = 0;
        
        // The remainder 0 is considered to have occurred at index -1.
        // This handles cases where a prefix sum from arr[0] to arr[i] is divisible by k.
        // In such a case, the subarray starts at index 0 and ends at index i,
        // and its length is i - (-1) = i + 1.
        first_occurrence[0] = -1;
        
        for (int i = 0; i < n; ++i) {
            current_sum += arr[i];
            
            // Calculate the remainder.
            // The expression (current_sum % k + k) % k ensures the remainder is always non-negative,
            // even if current_sum is negative. For example, -5 % 3 in C++ is -2.
            // (-2 + 3) % 3 = 1 % 3 = 1, which is the desired positive remainder.
            int remainder = (current_sum % k + k) % k;
            
            // If this remainder has been seen before:
            if (first_occurrence[remainder] != -2) {
                // Calculate the length of the subarray whose sum is divisible by k.
                // The sum from first_occurrence[remainder] + 1 to i is divisible by k.
                // Its length is i - (first_occurrence[remainder] + 1) + 1 = i - first_occurrence[remainder].
                max_len = std::max(max_len, i - first_occurrence[remainder]);
            } else {
                // If this is the first time we see this remainder, store its current index.
                first_occurrence[remainder] = i;
            }
        }
        
        return max_len;
    }
};