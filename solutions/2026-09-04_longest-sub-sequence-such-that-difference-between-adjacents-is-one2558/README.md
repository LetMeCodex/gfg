# Longest Subsequence with Adjacent Diff as 1

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

Given an array `arr` of integers, find the length of the longest subsequence such that the absolute difference between any two adjacent elements in the subsequence is exactly 1.

For example, if `arr = [1, 2, 3, 4, 5]`, the longest subsequence is `[1, 2, 3, 4, 5]` with length 5.
If `arr = [10, 9, 4, 5, 4, 8, 6]`, a possible longest subsequence is `[4, 5, 4]` with length 3.

## Intuition & Approach

The problem asks for the longest subsequence where adjacent elements differ by 1. This suggests a dynamic programming approach. We need to keep track of the longest subsequence ending at a particular number.

Let `dp[x]` be the length of the longest subsequence ending with the number `x`.
When we consider an element `arr[i]` (let's call it `x`), to form a subsequence ending with `x`, the previous element in the subsequence must be either `x-1` or `x+1`.

Therefore, the length of the longest subsequence ending with `x` can be derived from the lengths of the longest subsequences ending with `x-1` and `x+1`. Specifically, `dp[x]` will be `1 + max(dp[x-1], dp[x+1])`. The `+1` accounts for the current element `x` itself.

We iterate through the input array `arr`. For each element `x`, we calculate `dp[x]` based on the previously computed `dp` values for `x-1` and `x+1`. We also maintain a global `max_len` variable to store the maximum `dp[x]` encountered so far, which will be our final answer.

**Key Considerations:**

1.  **Array Bounds:** The problem statement implies that array elements can be up to $10^6$. This means our `dp` array needs to be large enough to accommodate values from 0 up to $10^6 + 1$. We need to handle `x-1` when `x=0` (though the problem states `arr[i] >= 1`, so `x-1` will be at least 0) and `x+1` when `x` is the maximum possible value.
2.  **Initialization:** The `dp` array should be initialized with zeros. A length of 0 signifies that no subsequence ending with that number has been found yet.
3.  **Order of Computation:** Since `dp[x]` depends on `dp[x-1]` and `dp[x+1]`, and we are iterating through the input array, we need to ensure that when we compute `dp[x]`, the values for `dp[x-1]` and `dp[x+1]` are already updated based on elements encountered *before* the current `x` in the input array. The DP approach naturally handles this because `dp[x-1]` and `dp[x+1]` would have been computed when those values were processed from the input array.

**Example Walkthrough:**

Let `arr = [1, 2, 3, 4, 5]`

Initialize `dp` array of size $10^6 + 2$ with all zeros. `max_len = 0`.

1.  **Process `x = 1`**:
    *   `dp[1] = 1 + max(dp[0], dp[2]) = 1 + max(0, 0) = 1`
    *   `max_len = max(0, 1) = 1`

2.  **Process `x = 2`**:
    *   `dp[2] = 1 + max(dp[1], dp[3]) = 1 + max(1, 0) = 2`
    *   `max_len = max(1, 2) = 2`

3.  **Process `x = 3`**:
    *   `dp[3] = 1 + max(dp[2], dp[4]) = 1 + max(2, 0) = 3`
    *   `max_len = max(2, 3) = 3`

4.  **Process `x = 4`**:
    *   `dp[4] = 1 + max(dp[3], dp[5]) = 1 + max(3, 0) = 4`
    *   `max_len = max(3, 4) = 4`

5.  **Process `x = 5`**:
    *   `dp[5] = 1 + max(dp[4], dp[6]) = 1 + max(4, 0) = 5`
    *   `max_len = max(4, 5) = 5`

Final `max_len = 5`.

Let `arr = [10, 9, 4, 5, 4, 8, 6]`

Initialize `dp` array of size $10^6 + 2$ with all zeros. `max_len = 0`.

1.  **Process `x = 10`**: `dp[10] = 1 + max(dp[9], dp[11]) = 1 + max(0, 0) = 1`. `max_len = 1`.
2.  **Process `x = 9`**: `dp[9] = 1 + max(dp[8], dp[10]) = 1 + max(0, 1) = 2`. `max_len = 2`.
3.  **Process `x = 4`**: `dp[4] = 1 + max(dp[3], dp[5]) = 1 + max(0, 0) = 1`. `max_len = 2`.
4.  **Process `x = 5`**: `dp[5] = 1 + max(dp[4], dp[6]) = 1 + max(1, 0) = 2`. `max_len = 2`.
5.  **Process `x = 4`**: `dp[4] = 1 + max(dp[3], dp[5]) = 1 + max(0, 2) = 3`. (Note: `dp[4]` is updated from 1 to 3). `max_len = 3`.
6.  **Process `x = 8`**: `dp[8] = 1 + max(dp[7], dp[9]) = 1 + max(0, 2) = 3`. `max_len = 3`.
7.  **Process `x = 6`**: `dp[6] = 1 + max(dp[5], dp[7]) = 1 + max(2, 0) = 3`. `max_len = 3`.

Final `max_len = 3`.

The solution uses a `std::vector<int> dp` of size `MAX_ARR_VAL + 2` to store the lengths. `MAX_ARR_VAL` is $10^6$. The indices `x-1` and `x+1` are always within the bounds of this vector because `arr[i] >= 1` implies `x-1 >= 0`, and `x <= 10^6` implies `x+1 <= 10^6 + 1`.

## Complexity Analysis

-   **Time Complexity**: $O(N + M)$, where $N$ is the number of elements in the input array `arr`, and $M$ is the maximum possible value an element can take (which is $10^6$ in this problem). We iterate through the input array once ($O(N)$). For each element, we perform constant time lookups and updates in the `dp` array. The size of the `dp` array is fixed at $M+2$. Therefore, the dominant factor is the iteration through the input array and the fixed size of the DP table. If $N$ is much larger than $M$, it's effectively $O(N)$. If $M$ is much larger than $N$, it's effectively $O(M)$ due to initialization. However, since the DP table is initialized once and then accessed, the overall time complexity is $O(N + M)$. Given $M$ is a constant ($10^6$), it can be considered $O(N)$.

-   **Space Complexity**: $O(M)$, where $M$ is the maximum possible value an element can take ($10^6$). This is due to the `dp` array which has a size proportional to the maximum possible value in the input array.

## Solution Code

```cpp
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
```