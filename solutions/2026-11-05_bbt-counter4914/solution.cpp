#include <vector>

class Solution {
public:
    // Renamed the method to match the driver code's expectation.
    int countBT(int h) {
        long long MOD = 1e9 + 7;

        // dp[i] will store the number of balanced binary trees of height exactly i.
        // We need dp array up to h.
        std::vector<long long> dp(h + 1, 0);

        // Base cases
        // An empty tree has height 0.
        dp[0] = 1;
        // A single node tree has height 1.
        if (h >= 1) {
            dp[1] = 1;
        }

        // Fill the dp table
        for (int i = 2; i <= h; ++i) {
            // For a tree of height i, the root node exists.
            // The left and right subtrees must have heights such that
            // the overall tree height is i and it's balanced.
            // The possible height combinations for (left_height, right_height) are:
            // 1. (i-1, i-1): Both subtrees have height i-1.
            //    Number of ways: dp[i-1] * dp[i-1]
            // 2. (i-1, i-2): Left subtree has height i-1, right has i-2.
            //    Number of ways: dp[i-1] * dp[i-2]
            // 3. (i-2, i-1): Left subtree has height i-2, right has i-1.
            //    Number of ways: dp[i-2] * dp[i-1]

            // Calculate ways for case 1
            long long case1 = (dp[i - 1] * dp[i - 1]) % MOD;

            // Calculate ways for case 2
            long long case2 = (dp[i - 1] * dp[i - 2]) % MOD;

            // Calculate ways for case 3
            long long case3 = (dp[i - 2] * dp[i - 1]) % MOD;

            // Total ways for height i is the sum of ways for these cases.
            // Ensure intermediate sums are also taken modulo MOD.
            dp[i] = (case1 + case2 + case3) % MOD;
        }

        return dp[h];
    }
};