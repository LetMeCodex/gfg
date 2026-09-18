#include <string>
#include <vector>
#include <algorithm>

class Solution {
public:
    /**
     * @brief Calculates the minimum cost to make two strings identical by deleting characters.
     *
     * The problem is equivalent to finding the Longest Common Subsequence (LCS).
     * Any character not in the LCS must be deleted from its respective string.
     * Total Cost = (len1 - LCS_length) * costS1 + (len2 - LCS_length) * costS2.
     *
     * @param s1 The first input string.
     * @param s2 The second input string.
     * @param costS1 The cost of deleting one character from s1.
     * @param costS2 The cost of deleting one character from s2.
     * @return The minimum cost required to make the two strings identical.
     */
    long long findMinCost(std::string s1, std::string s2, int costS1, int costS2) {
        int len1 = s1.length();
        int len2 = s2.length();

        // dp[i][j] stores the length of the LCS of s1[0...i-1] and s2[0...j-1].
        std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1, 0));

        for (int i = 1; i <= len1; ++i) {
            for (int j = 1; j <= len2; ++j) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                } else {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        int lcs_length = dp[len1][len2];

        // Calculate the minimum total cost using long long to prevent overflow.
        long long total_cost = (long long)(len1 - lcs_length) * costS1 + 
                               (long long)(len2 - lcs_length) * costS2;

        return total_cost;
    }
};