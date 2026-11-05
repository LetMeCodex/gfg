# Balanced Binary Tree Counter

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to count the number of distinct balanced binary trees of a given height `h`. A balanced binary tree is defined as a binary tree where for every node, the height difference between its left and right subtrees is at most 1.

## Intuition & Approach

This problem has a clear recursive structure and can be efficiently solved using dynamic programming. Let `dp[i]` represent the number of balanced binary trees of height exactly `i`.

To construct a balanced binary tree of height `i`, we consider the root node. The left and right subtrees of this root node must also be balanced binary trees. The height of the overall tree `i` is determined by the maximum height of its subtrees plus one (for the root).

For a tree to have height `i`, the heights of its left and right subtrees can be:

1.  **Left subtree height = `i-1`, Right subtree height = `i-1`**: In this case, the overall height is `max(i-1, i-1) + 1 = i`. The number of ways to form such a tree is `dp[i-1] * dp[i-1]`.

2.  **Left subtree height = `i-1`, Right subtree height = `i-2`**: In this case, the overall height is `max(i-1, i-2) + 1 = i`. The number of ways to form such a tree is `dp[i-1] * dp[i-2]`.

3.  **Left subtree height = `i-2`, Right subtree height = `i-1`**: In this case, the overall height is `max(i-2, i-1) + 1 = i`. The number of ways to form such a tree is `dp[i-2] * dp[i-1]`.

Therefore, the total number of balanced binary trees of height `i` is the sum of the ways for these three cases:
`dp[i] = (dp[i-1] * dp[i-1]) + (dp[i-1] * dp[i-2]) + (dp[i-2] * dp[i-1])`

We need to perform all calculations modulo `10^9 + 7` to prevent integer overflow.

**Base Cases:**

*   `dp[0] = 1`: There is one balanced binary tree of height 0, which is the empty tree.
*   `dp[1] = 1`: There is one balanced binary tree of height 1, which is a single node.

We can then iterate from height 2 up to `h`, calculating `dp[i]` using the recurrence relation.

The provided solution implements this dynamic programming approach. It initializes a `dp` vector of size `h+1`, sets the base cases `dp[0]` and `dp[1]`, and then iterates from `i = 2` to `h` to fill the `dp` table. The modulo operation is applied at each step of the calculation to ensure the result stays within the required bounds.

## Complexity Analysis

*   **Time Complexity**: $O(h)$
    The solution involves a single loop that iterates from 2 up to `h`. Inside the loop, constant time arithmetic operations are performed. Therefore, the time complexity is directly proportional to the height `h`.

*   **Space Complexity**: $O(h)$
    The solution uses a `std::vector` named `dp` of size `h + 1` to store the number of balanced binary trees for each height from 0 to `h`. This space requirement grows linearly with the height `h`.

## Solution Code

```cpp
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
```