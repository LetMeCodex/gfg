# [Smallest Subarray Sum Greater Than x](https://www.geeksforg eeks.org/problems/smallest-subarray-with-sum-greater-than-x5651/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

Given an array `arr` of `N` positive integers and an integer `x`, the task is to find the length of the smallest subarray whose sum is strictly greater than `x`. If no such subarray exists, return `0`.

**Example:**
- `arr = [1, 4, 45, 6, 0, 19]`, `x = 51`
- Output: `3` (The subarray `[45, 6, 0]` has sum `51`, which is not strictly greater than `x`. The subarray `[45, 6, 0, 19]` has sum `70 > 51`, length `4`. The subarray `[4, 45, 6]` has sum `55 > 51`, length `3`. This is the smallest.)

## Intuition & Approach

The problem asks for the *smallest* subarray satisfying a certain sum condition. This is a classic indicator for using the **Sliding Window** technique. Since all elements in the array are positive, adding an element always increases the sum, and removing an element always decreases it. This property is crucial for the effectiveness of the sliding window.

Here's the intuition behind the approach:

1.  **Expand the Window:** We use two pointers, `left` and `right`, to define our current subarray (window). We start by expanding the window from the right, adding elements one by one to a `current_sum`.
2.  **Check Condition:** As we expand, we continuously check if `current_sum` becomes strictly greater than `x`.
3.  **Shrink and Optimize:** Once `current_sum > x`, we have found a valid subarray. Now, our goal is to find the *smallest* such subarray. To do this, we try to shrink the window from the `left` side. As long as `current_sum` remains strictly greater than `x` after removing `arr[left]`, we can potentially make the subarray even smaller. Each time we shrink the window from the left while maintaining `current_sum > x`, we update our `min_length` with the current window's length (`right - left + 1`).
4.  **Repeat:** We continue this process: expand the window from the right, and if the sum condition is met, shrink from the left to find the minimum length.

**Detailed Steps:**

1.  Initialize `left = 0` (start of the window), `current_sum = 0` (sum of elements in the current window), and `min_length = n + 1` (an initial value larger than any possible subarray length, indicating no solution found yet).
2.  Iterate with the `right` pointer from `0` to `n-1`:
    a.  Add `arr[right]` to `current_sum`.
    b.  While `current_sum` is strictly greater than `x`:
        i.  Update `min_length` with the minimum of its current value and the current window's length (`right - left + 1`).
        ii. Subtract `arr[left]` from `current_sum` to shrink the window from the left.
        iii. Increment `left` to move the window's start.
3.  After the loop finishes, if `min_length` is still `n + 1`, it means no subarray was found whose sum is strictly greater than `x`. In this case, return `0`.
4.  Otherwise, return `min_length`.

**Why `long long` for `current_sum`?**
The problem constraints state `N` can be up to `10^5` and `arr[i]` up to `10^4`. In the worst case, the sum of all elements could be `10^5 * 10^4 = 10^9`. A standard 32-bit signed integer typically has a maximum value of `~2 * 10^9`. While `10^9` fits, using `long long` (which can hold up to `~9 * 10^18`) is a safer practice in competitive programming to prevent potential overflow issues with slightly larger constraints or intermediate calculations.

## Complexity Analysis

-   **Time Complexity**: $O(N)$
    -   The `right` pointer iterates from `0` to `N-1` exactly once.
    -   The `left` pointer also moves forward, from `0` to `N-1`, at most once over the entire execution.
    -   Each element of the array is added to `current_sum` once (by the `right` pointer) and subtracted from `current_sum` at most once (by the `left` pointer).
    -   Therefore, the total number of operations is proportional to `N`.

-   **Space Complexity**: $O(1)$
    -   We are only using a few constant extra variables (`left`, `right`, `current_sum`, `min_length`). The space used does not depend on the input array size `N`.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // Required for std::min

class Solution {
public:
    // Function to find the length of the smallest subarray whose sum is strictly greater than x.
    // Renamed function from 'smallestSubarraySum' to 'smallestSubWithSum'
    // and swapped parameter order to match the GeeksforGeeks driver code.
    int smallestSubWithSum(std::vector<int>& arr, int x) {
        int n = arr.size();
        
        // Initialize two pointers for the sliding window
        int left = 0;
        
        // current_sum can potentially be up to 10^5 * 10^4 = 10^9.
        // A 32-bit signed int can hold up to ~2 * 10^9, so it fits.
        // However, using long long is safer for sums in competitive programming
        // to prevent overflow issues with slightly larger constraints.
        long long current_sum = 0; 
        
        // Initialize min_length to a value greater than any possible subarray length.
        // n+1 is a safe choice, as the maximum possible length is n.
        int min_length = n + 1; 
        
        // Iterate with the right pointer to expand the window
        for (int right = 0; right < n; ++right) {
            // Add the current element to the window sum
            current_sum += arr[right];
            
            // While the current sum is strictly greater than x,
            // we have a candidate subarray. Try to shrink it from the left
            // to find the smallest possible valid subarray ending at 'right'.
            while (current_sum > x) {
                // Update min_length with the current subarray's length
                min_length = std::min(min_length, right - left + 1);
                
                // Shrink the window from the left
                current_sum -= arr[left];
                left++;
            }
        }
        
        // If min_length is still n + 1, it means no subarray was found
        // whose sum is strictly greater than x.
        if (min_length == n + 1) {
            return 0;
        } else {
            return min_length;
        }
    }
};
```