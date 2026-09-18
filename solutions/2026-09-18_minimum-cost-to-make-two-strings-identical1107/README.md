# [Min Cost To Make Two Strings Identical](https://www.geeksforgeeks.org/problems/minimum-cost-to-make-two-strings-identical1107/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary
Given two strings `s1` and `s2`, and two integers `costS1` and `costS2`, find the minimum cost to make both strings identical. You are allowed to delete characters from either string. Deleting a character from `s1` costs `costS1`, and deleting a character from `s2` costs `costS2`.

## Intuition & Approach
To make two strings identical with the minimum number of deletions, we must retain the longest possible sequence of characters that appear in both strings in the same relative order. This is the definition of the **Longest Common Subsequence (LCS)**.

1. **Identify the LCS**: Any character that is not part of the LCS must be deleted to make the strings identical.
2. **Calculate Deletions**:
   - Number of characters to delete from `s1` = `length(s1) - length(LCS)`
   - Number of characters to delete from `s2` = `length(s2) - length(LCS)`
3. **Calculate Cost**:
   - `Total Cost = (deletions_s1 * costS1) + (deletions_s2 * costS2)`
4. **Implementation**: We use Dynamic Programming to compute the LCS length. We create a 2D table `dp[len1+1][len2+1]` where `dp[i][j]` stores the LCS length of `s1[0...i-1]` and `s2[0...j-1]`.

## Complexity Analysis
- **Time Complexity**: $O(N \times M)$, where $N$ and $M$ are the lengths of strings `s1` and `s2` respectively. We iterate through the 2D DP table once.
- **Space Complexity**: $O(N \times M)$ to store the DP table. (Note: This can be optimized to $O(\min(N, M))$ using two rows, but $O(N \times M)$ is sufficient for standard constraints).

## Solution Code

```cpp
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
```