#include <vector>
#include <algorithm> // Required for std::max

class Solution {
public:
    // Renamed the function from 'longestSubsequence' to 'longestSubseq'
    // to match the name expected by the GeeksforGeeks driver code.
    int longestSubseq(std::vector<int>& arr) {
        // The maximum possible value for an element in arr[i] is 10^6.
        // We need a DP array to store the maximum length of a subsequence ending with a particular value.
        // dp[val] will store the length of the longest subsequence ending with 'val'.
        // The size of the dp array needs to accommodate values from 0 (for x-1 when x=1)
        // up to MAX_ARR_VAL + 1 (for x+1 when x=MAX_ARR_VAL).
        const int MAX_ARR_VAL = 1000000;
        
        // Initialize dp array with size MAX_ARR_VAL + 2, all elements set to 0.
        // This covers indices from 0 to MAX_ARR_VAL + 1.
        std::vector<int> dp(MAX_ARR_VAL + 2, 0);

        // This variable will keep track of the overall maximum length found across all elements.
        int max_len = 0;

        // Iterate through each element 'x' in the input array 'arr'.
        for (int x : arr) {
            // To form a subsequence ending with 'x', the previous element must be 'x-1' or 'x+1'.
            // We look up the longest subsequences ending with 'x-1' and 'x+1' that we've found so far.
            
            // len_x_minus_1: Length of the longest subsequence ending with 'x-1'.
            // Since arr[i] >= 1, x-1 will always be >= 0, so dp[x-1] is a valid index.
            int len_x_minus_1 = dp[x - 1];

            // len_x_plus_1: Length of the longest subsequence ending with 'x+1'.
            // Since x <= MAX_ARR_VAL, x+1 will always be <= MAX_ARR_VAL + 1, so dp[x+1] is a valid index.
            int len_x_plus_1 = dp[x + 1];

            // The new length of a subsequence ending with 'x' is 1 (for 'x' itself)
            // plus the maximum of the lengths of subsequences ending with 'x-1' or 'x+1'.
            // We update dp[x] with this new length. If 'x' appears multiple times,
            // this ensures dp[x] always stores the maximum possible length found so far.
            dp[x] = 1 + std::max(len_x_minus_1, len_x_plus_1);
            
            // Update the overall maximum length found if the current dp[x] is greater.
            max_len = std::max(max_len, dp[x]);
        }

        // After processing all elements, max_len will hold the length of the
        // longest subsequence satisfying the given condition.
        return max_len;
    }
};