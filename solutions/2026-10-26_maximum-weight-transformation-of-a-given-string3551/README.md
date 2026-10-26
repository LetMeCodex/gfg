# [Maximum Weight Transformation of String](https://www.geeksforgeeks.org/problems/maximum-weight-transformation-of-a-given-string3551/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary

The problem asks us to find the maximum possible weight of a string transformation. We are given a binary string `s` (containing only '0's and '1's). We can perform two types of operations:

1.  **Toggle a character**: Change a '0' to a '1' or a '1' to a '0'. This operation has a cost of 1.
2.  **Form a pair**: If two adjacent characters are the same (e.g., "00" or "11"), we can transform them into alternating characters (e.g., "01" or "10"). This operation has a weight of 4. If two adjacent characters are already alternating (e.g., "01" or "10"), we can also consider them as a pair, and this operation has a weight of 4 without any toggle cost.

The goal is to maximize the total weight obtained from these operations.

## Intuition & Approach

This problem screams Dynamic Programming. We need to make decisions at each step that depend on previous decisions, and we want to optimize a value (the total weight).

Let `dp[i]` represent the maximum weight achievable for the prefix of the string `s` of length `i` (i.e., `s[0...i-1]`). Our goal is to find `dp[n]`, where `n` is the length of the string `s`.

We can build our DP table iteratively. For each position `i` (from 1 to `n`), we consider how the character `s[i-1]` can contribute to the maximum weight. There are two main possibilities for the prefix of length `i`:

1.  **The character `s[i-1]` is treated as a standalone character.**
    In this case, the character `s[i-1]` itself contributes a weight of 1. The maximum weight for the prefix of length `i` would then be the maximum weight of the prefix of length `i-1` plus the weight of the current character: `dp[i-1] + 1`.

2.  **The character `s[i-1]` forms a pair with the previous character `s[i-2]`.**
    This option is only available if `i >= 2`. When we consider `s[i-2]` and `s[i-1]` as a pair, we need to determine the cost/weight associated with this pair.
    *   If `s[i-2]` and `s[i-1]` are the same (e.g., "00" or "11"), we need one toggle operation to make them alternating. The pair itself has a base weight of 4, and the toggle costs 1. So, the net contribution from this pair is `4 - 1 = 3`. The total weight would be `dp[i-2] + 3`.
    *   If `s[i-2]` and `s[i-1]` are already different (e.g., "01" or "10"), no toggle is needed. The pair contributes a weight of 4. The total weight would be `dp[i-2] + 4`.

At each step `i`, `dp[i]` will be the maximum of these possible options.

**Base Cases:**

*   `dp[0] = 0`: An empty string has a weight of 0.
*   `dp[1] = 1`: A string with a single character has a weight of 1 (the character itself).

**Transitions:**

For `i` from 2 to `n`:
`dp[i] = max(dp[i-1] + 1, dp[i-2] + pair_weight)`

where `pair_weight` is:
*   3 if `s[i-2] == s[i-1]`
*   4 if `s[i-2] != s[i-1]`

**Example Walkthrough:**

Let `s = "010"`

*   `n = 3`
*   `dp` array of size `n+1 = 4`.

*   `dp[0] = 0` (empty string)
*   `dp[1] = 1` (for "0")

*   **i = 2**: Consider prefix "01" (`s[0...1]`)
    *   Option 1 (standalone `s[1]`): `dp[1] + 1 = 1 + 1 = 2`
    *   Option 2 (pair `s[0]s[1]`): `s[0] = '0'`, `s[1] = '1'`. They are different. `pair_weight = 4`.
        `dp[0] + 4 = 0 + 4 = 4`
    *   `dp[2] = max(2, 4) = 4`

*   **i = 3**: Consider prefix "010" (`s[0...2]`)
    *   Option 1 (standalone `s[2]`): `dp[2] + 1 = 4 + 1 = 5`
    *   Option 2 (pair `s[1]s[2]`): `s[1] = '1'`, `s[2] = '0'`. They are different. `pair_weight = 4`.
        `dp[1] + 4 = 1 + 4 = 5`
    *   `dp[3] = max(5, 5) = 5`

The maximum weight for "010" is 5.

Let `s = "000"`

*   `n = 3`
*   `dp` array of size `n+1 = 4`.

*   `dp[0] = 0`
*   `dp[1] = 1` (for "0")

*   **i = 2**: Consider prefix "00" (`s[0...1]`)
    *   Option 1 (standalone `s[1]`): `dp[1] + 1 = 1 + 1 = 2`
    *   Option 2 (pair `s[0]s[0]`): `s[0] = '0'`, `s[1] = '0'`. They are the same. `pair_weight = 3`.
        `dp[0] + 3 = 0 + 3 = 3`
    *   `dp[2] = max(2, 3) = 3`

*   **i = 3**: Consider prefix "000" (`s[0...2]`)
    *   Option 1 (standalone `s[2]`): `dp[2] + 1 = 3 + 1 = 4`
    *   Option 2 (pair `s[1]s[2]`): `s[1] = '0'`, `s[2] = '0'`. They are the same. `pair_weight = 3`.
        `dp[1] + 3 = 1 + 3 = 4`
    *   `dp[3] = max(4, 4) = 4`

The maximum weight for "000" is 4.

The provided solution code implements this exact DP approach. The variable `cost_for_pair` correctly calculates the net contribution of a pair based on whether the characters are the same or different.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    The solution iterates through the string once to fill the DP table. The loop runs from `i = 2` to `n`, performing constant time operations within each iteration. Therefore, the time complexity is linear with respect to the length of the string `N`.

*   **Space Complexity**: $O(N)$
    The solution uses a DP table (a `std::vector<long long>`) of size `N+1` to store the maximum weights for prefixes of the string. This results in a space complexity that is linear with respect to the length of the string `N`.

## Solution Code

```cpp
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
```