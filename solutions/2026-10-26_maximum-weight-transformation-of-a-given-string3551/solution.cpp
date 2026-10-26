#include <string>
#include <algorithm> // Required for std::max
#include <vector> // Required for std::vector

class Solution {
public:
    // Renamed from maxWeightTransformation to getMaxWeight to match driver code
    long long getMaxWeight(std::string s) {
        int n = s.length();

        // Handle empty string case
        if (n == 0) {
            return 0;
        }

        // dp[i] will store the maximum weight for the prefix of length i (s[0...i-1]).
        // We use a vector for DP table.
        std::vector<long long> dp(n + 1, 0);

        // Base case for dp[0]: empty string, weight 0.
        dp[0] = 0;

        // Base case for dp[1]: string of length 1 (s[0]).
        // s[0] contributes 1 as a standalone character.
        dp[1] = 1;

        // Iterate from i = 2 up to n (inclusive)
        // i represents the current length of the prefix s[0...i-1]
        for (int i = 2; i <= n; ++i) {
            // Option 1: The character s[i-1] is a standalone character.
            // Its contribution is 1.
            // The maximum weight for s[0...i-1] would be dp[i-1] + 1.
            dp[i] = dp[i - 1] + 1;

            // Option 2: The character s[i-1] forms a pair with s[i-2].
            // This pair s[i-2]s[i-1] contributes to the weight.
            // The previous state would be dp[i-2].
            long long cost_for_pair;
            if (s[i - 2] == s[i - 1]) {
                // Characters are the same (e.g., "AA", "BB").
                // We need 1 toggle to make them alternating (e.g., "AB" or "BA").
                // Pair contributes 4, toggle costs 1. Net contribution = 4 - 1 = 3.
                cost_for_pair = 3;
            } else {
                // Characters are already alternating (e.g., "AB", "BA").
                // No toggles needed.
                // Pair contributes 4. Net contribution = 4 - 0 = 4.
                cost_for_pair = 4;
            }

            // Take the maximum of the two options
            dp[i] = std::max(dp[i], dp[i - 2] + cost_for_pair);
        }

        // After the loop, dp[n] holds the maximum weight for the entire string s (length n).
        return dp[n];
    }
};