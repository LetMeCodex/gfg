# [Count Indices That Escape Array](https://www.geeksforgfg.org/problems/good-stones--170647/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

Given an array `arr` of `N` integers, where `arr[i]` represents a jump value from index `i`. From any index `i`, you jump to `i + arr[i]`. An index `i` is considered "good" if, starting from `i`, you can eventually jump out of the array bounds (i.e., reach an index `< 0` or `>= N`). If, however, starting from `i`, you enter an infinite loop (cycle) or reach an index that leads to an infinite loop, then `i` is not a "good" index. The task is to count the total number of "good" indices in the array.

## Intuition & Approach

### Intuition

The problem asks us to determine the "fate" of each starting index: does it lead to an escape, or does it lead to an infinite loop? This kind of problem, involving paths, cycles, and determining a state for each node (index) based on its future path, is a classic candidate for Dynamic Programming (DP) or memoized recursion. Since there can be overlapping subproblems (multiple paths might lead to the same index), memoization is key to efficiency.

A crucial aspect is detecting cycles. If we are traversing a path and encounter an index that is *currently* in our recursion stack, it means we've found a cycle. Any index that leads to such a cycle is considered "bad".

### Approach (Memoized Recursion with Cycle Detection)

We can use a recursive helper function, `solve(current_idx)`, to determine the state of an index. To memoize results and detect cycles, we'll use a `dp` array with specific states:

1.  **`dp[i] = 0` (UNKNOWN):** The state of index `i` has not yet been determined.
2.  **`dp[i] = 1` (GOOD):** Index `i` eventually leads to an escape from the array.
3.  **`dp[i] = 2` (BAD):** Index `i` eventually leads to an infinite loop (cycle).
4.  **`dp[i] = 3` (VISITING):** Index `i` is currently in the recursion stack for the path being explored. This state is vital for detecting cycles.

**The `solve(current_idx)` function works as follows:**

1.  **Base Case 1 (Escape):** If `current_idx < 0` or `current_idx >= N`, it means we have successfully jumped out of the array. Return `1` (GOOD).
2.  **Base Case 2 (Memoized Result):** If `dp[current_idx]` is already `1` (GOOD) or `2` (BAD), it means we've already determined its fate. Return the stored value.
3.  **Base Case 3 (Cycle Detection):** If `dp[current_idx]` is `3` (VISITING), it means we have encountered this index again *within the current recursive path*. This signifies a cycle. Any index leading to a cycle is considered `BAD`. Return `2` (BAD).
4.  **Recursive Step:**
    *   Mark `dp[current_idx]` as `3` (VISITING). This is crucial to detect cycles that include `current_idx`.
    *   Calculate the `next_idx = current_idx + arr[current_idx]`.
    *   Recursively call `solve(next_idx)` to determine the state of the next index.
    *   Store the result of `solve(next_idx)` into `dp[current_idx]`. This updates the state from `VISITING` to its final `GOOD` or `BAD` state.
    *   Return `dp[current_idx]`.

**The main `goodIndices(arr)` function:**

1.  Initialize member variables `arr_global` (copy of input `arr`), `N` (size of `arr`), and `dp` array (all elements initialized to `0` (UNKNOWN)).
2.  Initialize `good_indices_count = 0`.
3.  Iterate through each index `i` from `0` to `N-1`:
    *   If `dp[i]` is `0` (UNKNOWN), call `solve(i)` to determine its state. (If its state was already determined by a previous path, we don't need to recompute).
    *   After `solve(i)` returns (or if `dp[i]` was already known), if `dp[i]` is `1` (GOOD), increment `good_indices_count`.
4.  Return `good_indices_count`.

This approach ensures that each index's state is computed only once, and cycles are correctly identified, leading to an efficient solution.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   The `goodIndices` function iterates `N` times. Inside the loop, `solve(i)` is called only if `dp[i]` is `UNKNOWN`.
    *   Each call to `solve(i)` (when `dp[i]` is `UNKNOWN`) explores a path. During this exploration, it marks indices as `VISITING` and eventually `GOOD` or `BAD`. Once an index's state is determined and memoized, subsequent calls to `solve` for that index (or any path leading to it) will simply return the memoized result in $O(1)$ time.
    *   Therefore, each index in the array is visited and processed by the `solve` function at most a constant number of times to determine its final state.
    *   The total work done is proportional to the number of indices, `N`.

*   **Space Complexity**: $O(N)$
    *   `arr_global`: Stores a copy of the input array, requiring $O(N)$ space.
    *   `dp`: Stores the state for each of the `N` indices, requiring $O(N)$ space.
    *   Recursion Stack: In the worst case (e.g., a long path that traverses almost all indices before escaping or forming a cycle), the recursion depth can be up to `N`. This also contributes $O(N)$ space.
    *   Combining these factors, the total space complexity is $O(N)$.

## Solution Code

```cpp
#include <vector> // Required for std::vector

class Solution {
private:
    // Member variables to store the input array, its size, and the DP states.
    // Using member variables avoids passing them through recursive calls,
    // making the code cleaner for competitive programming.
    std::vector<int> arr_global; 
    std::vector<int> dp;         
    int N;                        

    // States for dp array:
    // 0: UNKNOWN (not yet visited or processed)
    // 1: GOOD (eventually escapes the array)
    // 2: BAD (eventually enters an infinite loop)
    // 3: VISITING (currently in the recursion stack for the current path,
    //              used to detect cycles)

    // Helper function to determine the state of an index.
    // Returns 1 if GOOD, 2 if BAD.
    int solve(int current_idx) {
        // Base case 1: If current_idx is outside the array bounds, it means we escaped.
        if (current_idx < 0 || current_idx >= N) {
            return 1; // GOOD (escaped)
        }

        // Base case 2: Check memoized state to avoid redundant computations.
        if (dp[current_idx] == 1) { // Already known to be GOOD
            return 1;
        }
        if (dp[current_idx] == 2) { // Already known to be BAD
            return 2;
        }
        
        // Base case 3: If dp[current_idx] is 3 (VISITING), it means we have
        // encountered this index again in the current path. This indicates a cycle.
        // Any index leading to a cycle is considered BAD.
        if (dp[current_idx] == 3) { 
            return 2; // BAD (cycle detected)
        }

        // Mark current_idx as VISITING to detect cycles in the current path.
        dp[current_idx] = 3;

        // Calculate the next index based on the jump value.
        int next_idx = current_idx + arr_global[current_idx];

        // Recursively determine the state of the next index.
        int result = solve(next_idx);

        // Memoize the result for current_idx.
        // Once the recursive call returns, current_idx is no longer VISITING
        // for this specific path, and its final state (GOOD or BAD) is determined.
        dp[current_idx] = result;

        return result;
    }

public:
    // Main function to count good indices.
    // Renamed from countGoodIndices to goodIndices to match the GFG driver code.
    int goodIndices(std::vector<int>& arr) {
        // Copy the input array and its size to member variables for easy access
        // within the helper function. This copy happens once per test case.
        arr_global = arr; 
        N = arr.size();

        // Initialize the dp array with 0 (UNKNOWN) for all indices.
        dp.assign(N, 0);

        int good_indices_count = 0;

        // Iterate through all indices to determine their state.
        for (int i = 0; i < N; ++i) {
            // If the state of index 'i' is UNKNOWN, compute it using the solve function.
            // If it's already known (1 or 2), we don't need to recompute.
            if (dp[i] == 0) { 
                solve(i);
            }
            
            // After solve(i) (or if it was already computed), check its final state.
            // If dp[i] is 1, it means index 'i' is GOOD.
            if (dp[i] == 1) {
                good_indices_count++;
            }
        }

        return good_indices_count;
    }
};
```