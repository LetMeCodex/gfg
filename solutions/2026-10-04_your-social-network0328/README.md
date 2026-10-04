# [Your Social Network](https://www.geeksforgeeks.org/problems/your-social-network0328/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to model a social network where each user (except user 1) has exactly one friend. User 1 has no friend. We are given an array `arr` of size `N-1`, where `arr[i]` represents the friend of the `(i+2)`-th user. This means `arr[0]` is the friend of user 2, `arr[1]` is the friend of user 3, and so on, up to `arr[N-2]` being the friend of user `N`.

Our task is to find all possible triplets `[i, j, k]` such that:
1.  `i` is a starting user, ranging from 2 to `N`.
2.  `j` is a user reachable from `i` by following friend links.
3.  `j` must be strictly less than `i` (`j < i`).
4.  `k` is the number of friend links followed to reach `j` from `i`.

The output should be a list of these triplets, ordered first by increasing `i`, and for a fixed `i`, by increasing `j`.

**Example:**
If `N = 3` and `arr = {1, 2}`:
- User 2's friend is 1.
- User 3's friend is 2.

Output:
- For `i = 2`:
    - From 2, friend is 1. `j = 1`, `k = 1`. `1 < 2`. Triplet: `[2, 1, 1]`.
- For `i = 3`:
    - From 3, friend is 2. `j = 2`, `k = 1`. `2 < 3`. Triplet: `[3, 2, 1]`.
    - From 3, friend is 2, friend of 2 is 1. `j = 1`, `k = 2`. `1 < 3`. Triplet: `[3, 1, 2]`.
    - Ordered by `j`: `[3, 1, 2]`, `[3, 2, 1]`.

Final result: `[[2, 1, 1], [3, 1, 2], [3, 2, 1]]`.

## Intuition & Approach

The problem describes a directed graph where each node (user) has at most one outgoing edge (friend link). Since user 1 has no friend, all paths eventually lead to user 1 (or terminate if a cycle existed, but the problem structure implies a tree-like structure where user 1 is the root if edges are reversed). This structure is best represented using a `parent` array, where `parent[u]` stores the friend of user `u`.

1.  **Representing the Network**:
    *   We'll use a `std::vector<int> parent` of size `N+1` to store the friend relationships. We use 1-based indexing for users.
    *   `parent[1]` is initialized to `0`. This acts as a sentinel value, indicating that user 1 has no friend and marks the end of any friend-link chain.
    *   The input `arr` is used to populate the `parent` array: `parent[idx + 2] = arr[idx]` for `idx` from `0` to `N-2`.

2.  **Iterating Through Starting Users**:
    *   The problem requires triplets for `i` from 2 to `N` in increasing order. So, we'll use an outer loop `for (int i = 2; i <= N; ++i)`.

3.  **Traversing Friend Links for Each `i`**:
    *   For each starting user `i`, we need to find all reachable users `j` such that `j < i`.
    *   We start a traversal from `current_user = i` and `links_followed = 0`.
    *   In a `while` loop, we repeatedly update `current_user = parent[current_user]` and increment `links_followed`.
    *   The loop continues as long as `current_user` is not `0` (i.e., we haven't gone past user 1).
    *   Inside the loop, after updating `current_user` and `links_followed`, we check two conditions:
        *   `current_user >= 1`: Ensures `j` is a valid user.
        *   `current_user < i`: Ensures `j` meets the problem's criteria.
    *   If both conditions are met, we form the triplet `[i, current_user, links_followed]` and add it to a temporary list, say `current_i_reachables`.

4.  **Ordering `j` for a Fixed `i`**:
    *   The traversal `i -> parent[i] -> parent[parent[i]] -> ...` naturally finds reachable users `j` in decreasing order of their "depth" from `i` (i.e., closest friend first, then friend's friend, etc.). This means the `j` values are collected in decreasing order (e.g., for `i=5`, it might find `4`, then `2`, then `1`).
    *   The problem requires `j` to be in increasing order for a fixed `i`. To achieve this, after the `while` loop finishes for a given `i`, we `std::reverse` the `current_i_reachables` list.

5.  **Collecting Results**:
    *   Finally, we append all triplets from the (now reversed) `current_i_reachables` list to our main `result` vector.

This approach systematically finds all required triplets while adhering to the specified output order.

## Complexity Analysis

*   **Time Complexity**: $O(N^2)$
    *   **Initialization**: Populating the `parent` array takes $O(N)$ time.
    *   **Main Loop**: The outer loop iterates `N-1` times (for `i` from 2 to `N`).
    *   **Inner Traversal**: For each `i`, the `while` loop traverses the friend links until it reaches user 1 (or its sentinel `0`). In the worst case (a linear chain like `N -> N-1 -> ... -> 1`), this traversal can involve up to `N-1` steps. Each step involves a constant number of operations (array lookup, increment, comparison, `push_back`).
    *   **Total Traversal Operations**: Summing the lengths of paths from `i` to `1` for all `i` from 2 to `N`. In the worst-case scenario (a "skewed" tree where `parent[u] = u-1` for all `u > 1`), the path length for `i` is `i-1`. The total operations would be approximately `(1 + 2 + ... + (N-1))`, which is $O(N^2)$.
    *   **Reversing**: For each `i`, `std::reverse` operates on `current_i_reachables`. In the worst case, this list can contain up to `N-1` elements. Summing the costs of `reverse` for all `i` also leads to $O(N^2)$ operations.
    *   Therefore, the overall time complexity is dominated by the nested traversals, resulting in $O(N^2)$.

*   **Space Complexity**: $O(N^2)$
    *   **`parent` array**: Stores `N+1` integers, so $O(N)$ space.
    *   **`result` vector**: This vector stores all the `[i, j, k]` triplets. In the worst-case scenario (e.g., a linear chain `N -> N-1 -> ... -> 1`), the number of triplets generated is `(N-1) + (N-2) + ... + 1`, which is $O(N^2)$. Each triplet is a `std::vector<int>` of size 3. Thus, the `result` vector requires $O(N^2)$ space.
    *   **`current_i_reachables` vector**: This temporary vector stores triplets for a single `i`. In the worst case, it can hold up to `N-1` triplets, requiring $O(N)$ space.
    *   The dominant factor is the `result` vector, leading to an overall space complexity of $O(N^2)$.

**Note on Constraints**: Given `N <= 10^5`, an $O(N^2)$ solution would typically be too slow ($10^{10}$ operations) and consume too much memory ($10^{10}$ integers). However, this solution passed on GeeksforGeeks. This suggests that either the test cases are weak and do not hit the worst-case $O(N^2)$ for large `N`, or the effective maximum `N` for which the $O(N^2)$ worst-case occurs is much smaller (e.g., `N` up to a few thousands).

## Solution Code

```cpp
#include <vector>
#include <algorithm> // Required for std::reverse

class Solution {
public:
    // The problem statement implies the method name should be 'solve' based on the provided code.
    // However, the driver code in the diagnostic shows it expects 'socialNetwork'.
    // Renaming the method to 'socialNetwork' to match the driver.
    std::vector<std::vector<int>> socialNetwork(std::vector<int>& arr) {
        // N is the total number of users.
        // arr.size() is N-1, as it contains friends for users 2 to N.
        int N = arr.size() + 1;

        // parent[u] will store the friend of user u.
        // We use 1-based indexing for users, so the parent array size is N + 1.
        // parent[1] is set to 0 to indicate that user 1 has no friend and acts as a stopping point.
        std::vector<int> parent(N + 1);
        parent[1] = 0; 

        // Populate the parent array based on the input arr.
        // arr[idx] is the friend of user (idx + 2).
        // For example, arr[0] is friend of user 2, arr[1] is friend of user 3, etc.
        for (int idx = 0; idx < arr.size(); ++idx) {
            parent[idx + 2] = arr[idx];
        }

        std::vector<std::vector<int>> result;

        // Iterate through users 'i' from 2 to N as the starting user.
        for (int i = 2; i <= N; ++i) {
            // This temporary vector will store all reachable pairs [i, j, k] for the current 'i'.
            std::vector<std::vector<int>> current_i_reachables;
            
            int current_user = i;
            int links_followed = 0;

            // Traverse up the friend links until we reach user 1's "parent" (0).
            while (current_user != 0) {
                // Move to the friend of the current user.
                current_user = parent[current_user];
                links_followed++;

                // If the reachable user 'j' (current_user) is valid (1 <= j) and
                // has a smaller user number than 'i' (j < i), add it to our list.
                if (current_user >= 1 && current_user < i) {
                    current_i_reachables.push_back({i, current_user, links_followed});
                }
                // If current_user becomes 0, it means we've gone past user 1,
                // and the loop condition `current_user != 0` will terminate the traversal.
            }
            
            // The reachable users 'j' are collected in decreasing order during the traversal
            // (e.g., for i=3, we find 2 then 1).
            // The problem requires 'j' to be in increasing order.
            // Reversing the collected list achieves this.
            std::reverse(current_i_reachables.begin(), current_i_reachables.end());
            
            // Append all reachable pairs for user 'i' (now correctly ordered by 'j')
            // to the final result vector.
            for (const auto& entry : current_i_reachables) {
                result.push_back(entry);
            }
        }

        return result;
    }
};
```