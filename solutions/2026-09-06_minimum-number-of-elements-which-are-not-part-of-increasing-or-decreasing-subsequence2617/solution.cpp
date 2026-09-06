#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class Solution {
public:
    /**
     * The problem asks for the minimum number of elements not included in either 
     * a strictly increasing or a strictly decreasing subsequence.
     * This is equivalent to: Total elements - Maximum elements that can be 
     * partitioned into one increasing and one decreasing subsequence.
     * 
     * DP State: dp[inc][dec] = max elements used where the last element of the 
     * increasing subsequence is 'inc' and the last element of the decreasing 
     * subsequence is 'dec'.
     */
    int minCount(vector<int>& arr) {
        int n = arr.size();
        // dp[last_inc][last_dec] stores the max elements used so far.
        // last_inc: 0 to 100 (0 means empty)
        // last_dec: 0 to 101 (101 means empty, initialized to 101 as infinity)
        int dp[102][102];
        for(int i = 0; i < 102; ++i)
            for(int j = 0; j < 102; ++j)
                dp[i][j] = -1;

        dp[0][101] = 0;

        for (int x : arr) {
            int next_dp[102][102];
            memcpy(next_dp, dp, sizeof(dp));

            for (int inc = 0; inc <= 100; ++inc) {
                for (int dec = 0; dec <= 101; ++dec) {
                    if (dp[inc][dec] == -1) continue;

                    // Option 1: Add to increasing subsequence
                    if (x > inc) {
                        next_dp[x][dec] = max(next_dp[x][dec], dp[inc][dec] + 1);
                    }
                    // Option 2: Add to decreasing subsequence
                    if (x < dec) {
                        next_dp[inc][x] = max(next_dp[inc][x], dp[inc][dec] + 1);
                    }
                }
            }
            memcpy(dp, next_dp, sizeof(dp));
        }

        int max_used = 0;
        for (int i = 0; i <= 100; ++i)
            for (int j = 0; j <= 101; ++j)
                max_used = max(max_used, dp[i][j]);

        return n - max_used;
    }
};