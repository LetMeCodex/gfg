# [Adding Ones](https://www.geeksforgreeks.org/problems/adding-ones3628/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 3 attempt(s)

## Problem Summary

You are given an integer `n` and a vector `updates`.
Initially, you have an array `arr` of size `n` where all elements are 0.
For each value `j` in the `updates` vector (where `j` is 1-based), you need to add 1 to all elements `arr[i]` such that `i >= j` (again, `i` is 1-based).
Your task is to return the final state of the array `arr` after applying all updates.

**Example:**
`n = 3`, `updates = {1, 2}`
1. Initial `arr = {0, 0, 0}`
2. First update `j = 1`: Add 1 to `arr[1]`, `arr[2]`, `arr[3]`.
   `arr` becomes `{1, 1, 1}`
3. Second update `j = 2`: Add 1 to `arr[2]`, `arr[3]`.
   `arr` becomes `{1, 2, 2}`
Final result: `{1, 2, 2}`

## Intuition & Approach

### Brute Force (Inefficient)

A straightforward approach would be to simulate the process directly:
1. Initialize an array `arr` of size `n` with all zeros.
2. For each `j` in the `updates` vector:
   Iterate from `k = j-1` to `n-1` (using 0-based indexing for `arr`) and increment `arr[k]` by 1.
3. Return `arr`.

**Why it's inefficient:**
If `n` and the number of updates `k` are both large (e.g., $10^5$), and many updates start from `j=1`, each update would involve iterating through almost `n` elements. This leads to a time complexity of $O(k \cdot n)$, which would be $10^5 \cdot 10^5 = 10^{10}$ in the worst case, resulting in a Time Limit Exceeded (TLE) error.

### Optimized Approach: Difference Array

The problem involves multiple range updates. Specifically, each update is a "suffix update" (add 1 to all elements from an index to the end of the array). This type of problem is perfectly suited for the **Difference Array** technique.

**How Difference Array Works for Range Updates:**
To add a value `val` to a range `[L, R]` (0-based) in an array `A`:
1. Increment `diff[L]` by `val`. This marks the beginning of the increment.
2. Decrement `diff[R+1]` by `val`. This cancels out the increment effect *after* index `R`.
After processing all updates, the original array `A` can be reconstructed by calculating the prefix sums of the `diff` array. That is, `A[i] = diff[0] + diff[1] + ... + diff[i]`.

**Applying to this problem:**
Each update `j` (1-based) means "add 1 to `arr[i]` for `i >= j` (1-based)".
In 0-based indexing, this translates to "add 1 to `arr[i]` for `i` in the range `[j-1, n-1]`".
So, for each update:
- `L = j-1` (start of the range, 0-based)
- `R = n-1` (end of the range, 0-based)
- `val = 1`

Using the difference array technique:
1. Initialize a `diff` array of size `n+1` with all zeros. (We need `n+1` size because `R+1` can be `n`).
2. For each `update_idx` (1-based) in the `updates` vector:
   a. Increment `diff[update_idx - 1]` by 1. (This marks the start of the range `[update_idx - 1, n-1]`).
   b. Decrement `diff[n]` by 1. (This effectively stops the increment at `n-1`, as `diff[n]` will cancel out the accumulated sum for any index `i >= n`).
3. After processing all updates, compute the final `arr` by calculating prefix sums on the `diff` array:
   a. Initialize `current_add = 0`.
   b. For `i` from `0` to `n-1`:
      i. Add `diff[i]` to `current_add`.
      ii. Set `arr[i] = current_add`.
4. Return `arr`.

This approach processes each update in $O(1)$ time and then reconstructs the array in $O(n)$ time, leading to a much more efficient solution.

## Complexity Analysis

-   **Time Complexity**:
    -   Initializing the `diff` array takes $O(n)$ time.
    -   Processing `k` updates (where `k` is the size of the `updates` vector): Each update involves two $O(1)$ operations on the `diff` array. Total $O(k)$ time.
    -   Calculating prefix sums to construct the final `arr`: This involves a single loop from `0` to `n-1`, taking $O(n)$ time.
    -   Overall, the time complexity is $O(n + k)$. Given $n, k \le 10^5$, this is highly efficient.

-   **Space Complexity**:
    -   We use an auxiliary `diff` array of size `n+1`, which takes $O(n)$ space.
    -   The final `arr` vector also takes $O(n)$ space.
    -   Overall, the space complexity is $O(n)$.

## Solution Code

```cpp
#include <vector>
#include <numeric> // Not strictly needed for this solution, but often useful.

class Solution {
public:
    std::vector<int> update(int n, std::vector<int>& updates) {
        // arr will store the final result. It's initialized to zeros,
        // but its elements are populated later based on the diff array.
        std::vector<int> arr(n, 0);

        // diff array for efficient range updates.
        // Size n+1 to handle updates that affect elements up to index n-1 (0-based).
        // diff[i] stores the net change that starts at index i.
        std::vector<int> diff(n + 1, 0);

        // Process each update.
        // For each value 'j' in updates[] (1-based):
        // The problem states to add 1 to arr[i] for all i >= j (1-based).
        // In 0-based indexing, this means adding 1 to arr[i] for i >= j-1.
        // The range affected is [j-1, n-1] (0-based).
        for (int update_idx : updates) {
            // Increment at the start of the range (0-based index).
            // update_idx is 1-based, so j-1 is update_idx - 1.
            diff[update_idx - 1]++;

            // Decrement at the index *after* the end of the range.
            // The range ends at n-1. The index after is n.
            // This ensures the increment effect stops at arr[n-1].
            // The diff array has size n+1, so index n is valid.
            diff[n]--;
        }

        // Calculate prefix sums on the diff array to get the final array elements.
        // current_add accumulates the net change up to the current index.
        int current_add = 0;
        for (int i = 0; i < n; ++i) {
            current_add += diff[i]; // Apply the net change at index i
            arr[i] = current_add;   // Store the accumulated sum in arr[i]
        }

        return arr;
    }
};
```