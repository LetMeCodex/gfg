# Minimum Elements Outside Subsequences

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 4 attempt(s)

## Problem Summary

The problem asks us to find the minimum number of elements in a given array that are *not* part of any strictly increasing subsequence *or* any strictly decreasing subsequence. This means we want to maximize the number of elements that *can* be part of such subsequences.

## Intuition & Approach

The core idea is to rephrase the problem: instead of minimizing elements *outside* subsequences, we want to maximize elements *inside* subsequences. If we can find the maximum number of elements that can be part of *either* a strictly increasing subsequence *or* a strictly decreasing subsequence, then the minimum number of elements outside will be the total number of elements minus this maximum.

This problem can be modeled using dynamic programming. We need to keep track of the state of two potential subsequences: one increasing and one decreasing. For each element in the input array, we have a few choices:

1.  **Ignore the current element**: This element is not part of our chosen increasing or decreasing subsequence.
2.  **Add the current element to the increasing subsequence**: This is only possible if the current element is strictly greater than the last element added to the increasing subsequence.
3.  **Add the current element to the decreasing subsequence**: This is only possible if the current element is strictly less than the last element added to the decreasing subsequence.

The challenge with a direct DP approach like `dp[i][j]` representing the maximum length of an increasing subsequence ending at index `i` and a decreasing subsequence ending at index `j` is that it becomes difficult to combine these two.

A more effective DP state considers the *last elements* of the increasing and decreasing subsequences formed so far. Let `dp[last_inc][last_dec]` be the maximum number of elements we can use to form an increasing subsequence ending with `last_inc` and a decreasing subsequence ending with `last_dec`.

The values in the array are constrained to be between 1 and 100. This is a crucial observation that allows us to use the values themselves as indices in our DP table.

We can define our DP state as:
`dp[i][j]` = the maximum number of elements used such that the last element of the increasing subsequence is `i` and the last element of the decreasing subsequence is `j`.

To handle the cases where a subsequence is empty, we can use sentinel values.
*   For the increasing subsequence, we can use `0` to represent an empty subsequence (since all array elements are at least 1).
*   For the decreasing subsequence, we can use a value larger than any possible element, say `101`, to represent an empty subsequence.

The DP transition would iterate through each element `x` in the input array `arr`. For each `x`, we consider all possible previous states `dp[inc][dec]` and try to extend them:

*   **Option 1: Add `x` to the increasing subsequence.** If `x > inc`, we can potentially update `dp[x][dec]` to `dp[inc][dec] + 1`.
*   **Option 2: Add `x` to the decreasing subsequence.** If `x < dec`, we can potentially update `dp[inc][x]` to `dp[inc][dec] + 1`.

We need to use a temporary DP table (`next_dp`) to store the results of the current iteration to avoid using updated values within the same iteration.

The base case is `dp[0][101] = 0`, meaning with an empty increasing subsequence (last element 0) and an empty decreasing subsequence (last element 101), we have used 0 elements.

After iterating through all elements of `arr`, the maximum value in the `dp` table will represent the maximum number of elements that can be part of *either* an increasing *or* a decreasing subsequence. The final answer is `n - max_used`.

The constraints on the values (1 to 100) are critical. If the values were much larger, this DP approach would not be feasible due to memory limitations.

The provided solution uses `dp[102][102]` where indices 0-100 represent possible last elements of the increasing subsequence, and indices 0-100 represent possible last elements of the decreasing subsequence. The sentinel for an empty decreasing subsequence is `101`.

Let's trace the DP state and transitions more precisely with the code's indexing:
`dp[last_inc][last_dec]`
- `last_inc`: 0 to 100. `0` signifies an empty increasing subsequence.
- `last_dec`: 0 to 101. `101` signifies an empty decreasing subsequence.

Initialization: `dp[0][101] = 0`. All other `dp` entries are initialized to `-1` (or some indicator of an invalid state).

For each element `x` in `arr`:
Create `next_dp` as a copy of `dp`.
Iterate through all valid `inc` (0 to 100) and `dec` (0 to 101) states in `dp`.
If `dp[inc][dec]` is valid (not -1):
    1. **Add `x` to increasing subsequence**: If `x > inc`, then we can form a new state where the increasing subsequence ends with `x` and the decreasing subsequence still ends with `dec`. The number of elements used would be `dp[inc][dec] + 1`. We update `next_dp[x][dec] = max(next_dp[x][dec], dp[inc][dec] + 1)`.
    2. **Add `x` to decreasing subsequence**: If `x < dec`, then we can form a new state where the increasing subsequence still ends with `inc` and the decreasing subsequence now ends with `x`. The number of elements used would be `dp[inc][dec] + 1`. We update `next_dp[inc][x] = max(next_dp[inc][x], dp[inc][dec] + 1)`.

After processing all `inc` and `dec` for the current `x`, copy `next_dp` back to `dp`.

Finally, find the maximum value in `dp` across all `inc` and `dec` states. This `max_used` is the maximum number of elements that can be part of *at least one* of the subsequences. The answer is `n - max_used`.

The "4 attempt(s)" suggests that the initial approaches might have been incorrect or inefficient, possibly due to misinterpreting the problem or struggling with the DP state definition. The key insight is to use the values as indices and to track the *last elements* of the two subsequences simultaneously.

## Complexity Analysis

-   **Time Complexity**: $O(N \cdot V^2)$, where $N$ is the number of elements in the array and $V$ is the maximum possible value of an element (100 in this case).
    The outer loop iterates through each of the $N$ elements in the array.
    Inside this loop, we iterate through all possible states of `inc` (0 to 100) and `dec` (0 to 101). This is approximately $101 \times 102$ states.
    For each state, we perform constant time operations (comparisons and updates).
    Therefore, the total time complexity is $O(N \cdot 101 \cdot 102)$, which simplifies to $O(N \cdot V^2)$.

-   **Space Complexity**: $O(V^2)$, where $V$ is the maximum possible value of an element (100).
    We use a 2D DP table of size `102 x 102` to store the DP states. This space is independent of the input array size $N$.
    The `next_dp` table also takes $O(V^2)$ space.
    Therefore, the total space complexity is $O(102 \cdot 102)$, which simplifies to $O(V^2)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm>
#include <cstring> // For memcpy

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
     * 
     * Constraints: Array elements are between 1 and 100.
     * We use 0 for 'inc' to represent an empty increasing subsequence.
     * We use 101 for 'dec' to represent an empty decreasing subsequence.
     */
    int minCount(vector<int>& arr) {
        int n = arr.size();
        
        // dp[last_inc][last_dec] stores the max elements used so far.
        // last_inc: 0 to 100 (0 means empty)
        // last_dec: 0 to 101 (101 means empty, initialized to 101 as infinity)
        // We use a 2D array of size 102x102 to accommodate values 0-100 for inc
        // and 0-101 for dec.
        int dp[102][102];
        
        // Initialize DP table with -1 to indicate invalid states.
        // Using memset for efficiency.
        memset(dp, -1, sizeof(dp));

        // Base case: With an empty increasing subsequence (last element 0)
        // and an empty decreasing subsequence (last element 101), we have used 0 elements.
        dp[0][101] = 0;

        // Iterate through each element in the input array.
        for (int x : arr) {
            // Create a temporary DP table to store updates for the current element.
            // This prevents using values updated in the same iteration.
            int next_dp[102][102];
            memcpy(next_dp, dp, sizeof(dp));

            // Iterate through all possible previous states.
            // 'inc' represents the last element of the increasing subsequence.
            // 'dec' represents the last element of the decreasing subsequence.
            for (int inc = 0; inc <= 100; ++inc) {
                for (int dec = 0; dec <= 101; ++dec) {
                    // If the current state (inc, dec) is unreachable, skip it.
                    if (dp[inc][dec] == -1) continue;

                    // Option 1: Try to add the current element 'x' to the increasing subsequence.
                    // This is possible if 'x' is strictly greater than the current last element 'inc'.
                    if (x > inc) {
                        // Update the state where the increasing subsequence now ends with 'x'.
                        // The number of elements used increases by 1.
                        next_dp[x][dec] = max(next_dp[x][dec], dp[inc][dec] + 1);
                    }
                    
                    // Option 2: Try to add the current element 'x' to the decreasing subsequence.
                    // This is possible if 'x' is strictly less than the current last element 'dec'.
                    if (x < dec) {
                        // Update the state where the decreasing subsequence now ends with 'x'.
                        // The number of elements used increases by 1.
                        next_dp[inc][x] = max(next_dp[inc][x], dp[inc][dec] + 1);
                    }
                }
            }
            // After considering all possible transitions for the current element 'x',
            // update the main DP table with the results from next_dp.
            memcpy(dp, next_dp, sizeof(dp));
        }

        // After processing all elements, find the maximum number of elements used
        // across all possible final states.
        int max_used = 0;
        for (int i = 0; i <= 100; ++i) {
            for (int j = 0; j <= 101; ++j) {
                max_used = max(max_used, dp[i][j]);
            }
        }

        // The minimum number of elements not part of any subsequence is the total
        // number of elements minus the maximum number of elements that can be used.
        return n - max_used;
    }
};
```