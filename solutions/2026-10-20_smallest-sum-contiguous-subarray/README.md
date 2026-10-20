# GeeksforGeeks POTD: Minimum Sum Subarray

## Problem Link
- [Minimum Sum Subarray](https://www.geeksforgeeks.org/problems/smallest-sum-contiguous-subarray/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find the contiguous subarray within a given array of integers that has the smallest possible sum. We need to return this minimum sum.

## Intuition & Approach

This problem is a classic variation of Kadane's algorithm, which is typically used to find the maximum sum subarray. The core idea remains the same: we iterate through the array, maintaining a `current_min` sum and an overall `min_so_far` sum.

For each element `arr[i]`, we have two choices for forming the minimum sum subarray ending at `arr[i]`:

1.  **Start a new subarray at `arr[i]`**: The sum of this subarray would simply be `arr[i]`.
2.  **Extend the previous minimum sum subarray**: If the `current_min` sum ending at the previous element (`arr[i-1]`) is negative, adding `arr[i]` to it might result in a smaller sum than `arr[i]` alone. So, the sum would be `current_min + arr[i]`.

We choose the minimum of these two options to update `current_min`. This ensures that `current_min` always holds the minimum sum of a contiguous subarray ending at the current position.

Simultaneously, we update `min_so_far` by comparing it with the current `current_min`. This way, `min_so_far` keeps track of the smallest sum encountered across all possible contiguous subarrays throughout the iteration.

**Initialization**:
- `min_so_far` and `current_min` are initialized with the first element of the array (`arr[0]`). This is crucial because the array is guaranteed to have at least one element, and this initialization correctly handles the base case of a single-element array and sets up the loop for subsequent elements.

**Iteration**:
- We iterate from the second element (`i = 1`) to the end of the array.
- In each iteration, `current_min` is updated using `std::min(arr[i], current_min + arr[i])`.
- `min_so_far` is updated using `std::min(min_so_far, current_min)`.

**Result**:
- After the loop finishes, `min_so_far` will contain the minimum sum of any contiguous subarray in the given array.

The solution code provided implements this exact logic. The comment about the previous attempt highlights a common pitfall: ensuring the function name matches the judge's expectation.

## Complexity Analysis

-   **Time Complexity**: $O(N)$
    The algorithm iterates through the input array `arr` exactly once. For each element, a constant number of operations (comparisons and additions) are performed. Therefore, the time complexity is linear with respect to the size of the input array, $N$.

-   **Space Complexity**: $O(1)$
    The algorithm uses only a few extra variables (`min_so_far`, `current_min`, and the loop counter `i`) to store intermediate results. The amount of extra space used does not depend on the size of the input array. Hence, the space complexity is constant.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // Required for std::min

class Solution {
public:
    // The GeeksforGeeks judge expects a method named 'minSubarraySum'.
    // The previous attempt had 'minimumSumSubarray', leading to a compilation error.
    int minSubarraySum(std::vector<int>& arr) {
        // Constraints: 1 <= arr.size() <= 10^5
        // This means 'arr' will always contain at least one element,
        // so there's no need to handle an empty array case.

        // Initialize min_so_far and current_min with the first element of the array.
        // This correctly handles arrays with a single element and sets up the
        // initial state for the iteration.
        int min_so_far = arr[0];
        int current_min = arr[0];

        // Iterate through the array starting from the second element.
        // The loop variable 'i' is of type size_t, which is appropriate for vector indices.
        for (size_t i = 1; i < arr.size(); ++i) {
            // For each element arr[i], we decide whether to extend the current minimum
            // subarray or start a new one from arr[i].
            // We want the minimum of:
            // 1. arr[i] itself (starting a new subarray)
            // 2. current_min + arr[i] (extending the previous minimum subarray)
            // This is the core logic for finding the minimum sum subarray,
            // analogous to Kadane's algorithm for maximum sum.
            current_min = std::min(arr[i], current_min + arr[i]);

            // After updating current_min, we compare it with min_so_far to keep track
            // of the overall minimum sum found across all subarrays encountered so far.
            min_so_far = std::min(min_so_far, current_min);
        }

        // After iterating through all elements, min_so_far will hold the
        // minimum sum of any contiguous subarray.
        return min_so_far;
    }
};
```