Here's a high-quality GitHub solution writeup for the given problem:

---

# [Maximum Subarray Sum with One Square Allowed](https://www.geeksforgeeks.org/problems/maximum-sum-subarray--143510/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

Given an array `arr` of `N` integers, the task is to find the maximum possible sum of a subarray. The unique twist is that we are allowed to square *at most one* element within that chosen subarray.

**Constraints:**
- `1 <= N <= 10^5`
- `-1000 <= arr[i] <= 1000`

The sum can be large, so the result should be stored in a `long long` data type.

## Intuition & Approach

This problem is a classic dynamic programming problem, a variation of Kadane's algorithm. Kadane's algorithm efficiently finds the maximum sum of a contiguous subarray. Here, we have an additional choice: whether to square an element or not. This suggests that we need to maintain states based on this choice.

We can define two DP states for the maximum subarray sum ending at the current index `i`:

1.  **`max_so_far_no_square`**: The maximum sum of a subarray ending at the current position `i`, where *no* element in this subarray has been squared yet.
2.  **`max_so_far_one_square`**: The maximum sum of a subarray ending at the current position `i`, where *exactly one* element in this subarray has been squared.

Let's derive the recurrence relations:

**Initialization (for `i = 0`):**
For the first element `arr[0]`:
-   If no element is squared: `max_so_far_no_square = arr[0]`
-   If `arr[0]` itself is squared: `max_so_far_one_square = arr[0] * arr[0]`
The overall maximum sum `overall_max_sum` is initialized with the maximum of these two possibilities.

**Iteration (for `i > 0`):**
For each subsequent element `arr[i]`:

**1. Updating `max_so_far_no_square` (no element squared):**
To get the maximum sum ending at `i` without squaring any element, we have two choices:
-   Start a new subarray at `arr[i]`: The sum is `arr[i]`.
-   Extend the previous `max_so_far_no_square` by adding `arr[i]`: The sum is `max_so_far_no_square (from i-1) + arr[i]`.
We take the maximum of these two:
`new_max_so_far_no_square = std::max(arr[i], max_so_far_no_square + arr[i])`

**2. Updating `max_so_far_one_square` (exactly one element squared):**
To get the maximum sum ending at `i` with exactly one element squared, we have three possibilities:
-   **Extend a subarray that already had one element squared:** We add `arr[i]` (without squaring it) to the previous `max_so_far_one_square (from i-1)`.
    `max_so_far_one_square (from i-1) + arr[i]`
-   **Square `arr[i]` itself:** We take the `max_so_far_no_square (from i-1)` (a subarray ending at `i-1` with no squared elements) and add `arr[i] * arr[i]` to it. This is the first time an element is squared in the current subarray.
    `max_so_far_no_square (from i-1) + arr[i] * arr[i]`
-   **Square `arr[i]` itself and start a new subarray:** The subarray consists only of `arr[i]` squared.
    `arr[i] * arr[i]`
We take the maximum of these three:
`new_max_so_far_one_square = std::max({max_so_far_one_square + arr[i], max_so_far_no_square + arr[i]*arr[i], arr[i]*arr[i]})`

**Overall Maximum:**
After calculating `new_max_so_far_no_square` and `new_max_so_far_one_square` for the current `i`, we update our `overall_max_sum` with the maximum value seen so far, including these new sums.

**Space Optimization:**
Notice that the current DP states only depend on the previous states (`i-1`). This allows us to optimize the space complexity from $O(N)$ (if we stored full DP arrays) to $O(1)$ by simply using variables to store the previous `max_so_far_no_square` and `max_so_far_one_square` values.

**Data Type Consideration:**
Since `arr[i]` can be up to `1000`, `arr[i] * arr[i]` can be `1000 * 1000 = 10^6`. The array size `N` can be `10^5`. A sum like `10^5 * 1000 = 10^8` (without squaring) or `(10^5 - 1) * 1000 + 1000^2 = 10^8 - 1000 + 10^6 \approx 10^8` (with one square) can exceed `INT_MAX` (`~2 * 10^9`). Therefore, `long long` is essential for all sum variables to prevent overflow.

## Complexity Analysis

-   **Time Complexity**: $O(N)$
    The algorithm iterates through the array exactly once, performing a constant number of arithmetic operations and comparisons for each element. Thus, the time complexity is directly proportional to the number of elements `N` in the input array.

-   **Space Complexity**: $O(1)$
    The algorithm uses a fixed number of variables (`max_so_far_no_square`, `max_so_far_one_square`, `overall_max_sum`, and a few temporary variables within the loop) regardless of the input array size `N`. No auxiliary data structures that scale with `N` are used.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // Required for std::max

class Solution {
public:
    // Renamed from maxSumWithOneSquare to maximumSubarray to match the driver code.
    long long maximumSubarray(std::vector<int>& arr) {
        int n = arr.size();
        // Constraints state 1 <= arr.size(), so n will always be at least 1.

        // max_so_far_no_square: maximum subarray sum ending at current position,
        //                       where no element in the subarray has been squared yet.
        // max_so_far_one_square: maximum subarray sum ending at current position,
        //                        where exactly one element in the subarray has been squared.
        // overall_max_sum: the global maximum sum found across all subarrays and squaring options.

        // Initialize with the first element of the array.
        // For arr[0]:
        // 1. If arr[0] is part of a subarray with no squared elements: sum is arr[0].
        // 2. If arr[0] is the squared element in a subarray: sum is arr[0]*arr[0].
        long long max_so_far_no_square = arr[0];
        // Cast to long long before multiplication to prevent potential overflow if arr[0]*arr[0]
        // exceeds INT_MAX, though for arr[i] up to 1000, 1000*1000 = 10^6 fits in int.
        // It's good practice to ensure all intermediate calculations that might grow large use long long.
        long long max_so_far_one_square = (long long)arr[0] * arr[0]; 
        
        // The overall maximum sum must consider both possibilities for the first element.
        long long overall_max_sum = std::max(max_so_far_no_square, max_so_far_one_square);

        // Iterate from the second element to the end of the array.
        for (int i = 1; i < n; ++i) {
            long long current_val = arr[i];
            // current_val is long long, so current_val * current_val will be long long.
            long long current_val_sq = current_val * current_val; 

            // Calculate the new maximum subarray sum ending at 'i' without squaring any element.
            // This can either be 'current_val' itself (starting a new subarray)
            // or 'current_val' added to the previous max_so_far_no_square.
            long long new_max_so_far_no_square = std::max(current_val, max_so_far_no_square + current_val);

            // Calculate the new maximum subarray sum ending at 'i' with exactly one element squared.
            // This can be formed in three ways:
            // 1. Extend a subarray that already had one element squared: max_so_far_one_square + current_val
            //    (current_val is added normally, as one element was already squared before it).
            // 2. Square 'current_val' itself, extending a subarray that previously had no squared elements:
            //    max_so_far_no_square + current_val_sq
            //    (current_val is squared, and added to a sum that had no squared elements before 'arr[i]').
            // 3. Square 'current_val' itself, starting a new subarray: current_val_sq
            //    (current_val is squared, forming a new subarray of just this element).
            long long new_max_so_far_one_square = std::max({max_so_far_one_square + current_val, 
                                                              max_so_far_no_square + current_val_sq, 
                                                              current_val_sq});
            
            // Update the states for the next iteration.
            max_so_far_no_square = new_max_so_far_no_square;
            max_so_far_one_square = new_max_so_far_one_square;

            // Update the overall maximum sum found so far.
            // It could be the current max_so_far_no_square or max_so_far_one_square.
            overall_max_sum = std::max({overall_max_sum, max_so_far_no_square, max_so_far_one_square});
        }

        return overall_max_sum;
    }
};
```