# [Maximum Packages within Time](https://www.geeksforgreeks.org/problems/bird-and-maximum-fruit-gathering0509/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to find the maximum number of fruits (or packages, as per the solution code's variable names) a bird (or agent) can gather within a given `totalTime`. We are provided with an array `arr` where `arr[i]` represents the number of fruits on the `i`-th tree. The key constraint is that the bird can only gather fruits from *consecutive* trees. Visiting `k` consecutive trees takes `k` units of time. We need to maximize the total fruits gathered within `totalTime` units.

Essentially, we need to find a contiguous subarray (a "window" of trees) whose length is at most `totalTime` (and also at most the total number of trees `N`), such that the sum of fruits in this subarray is maximized.

## Intuition & Approach

### Intuition

The problem boils down to finding the maximum sum of elements within a subarray of a specific, fixed length. The length of this subarray, let's call it `k`, is determined by the `totalTime` constraint. Since visiting `k` trees takes `k` units of time, the bird can visit at most `totalTime` trees. Additionally, the bird cannot visit more trees than are available in the array (`N`). Therefore, the effective maximum number of consecutive trees the bird can visit is `k = std::min(totalTime, N)`.

Once we determine this fixed window size `k`, the problem becomes a classic "Maximum Subarray Sum of Fixed Size `k`" problem. This type of problem is efficiently solved using the **Sliding Window** technique.

### Approach

1.  **Determine the Window Size (`k`)**:
    *   The maximum number of consecutive trees the bird can visit is limited by `totalTime`.
    *   It's also limited by the total number of trees `N` (i.e., `arr.size()`).
    *   So, the effective window size `k` is `std::min(totalTime, N)`.

2.  **Calculate the Sum of the First Window**:
    *   Initialize `current_sum` to 0.
    *   Iterate from `i = 0` to `k-1` (the first `k` elements of the array) and add `arr[i]` to `current_sum`.
    *   Initialize `max_sum` with this `current_sum`, as it's the maximum sum found so far.

3.  **Slide the Window**:
    *   Iterate from `i = k` to `N-1`. This `i` represents the index of the new element entering the window from the right.
    *   In each iteration, to slide the window one position to the right:
        *   **Remove the leftmost element**: Subtract `arr[i - k]` from `current_sum`. This element is now outside the window.
        *   **Add the new rightmost element**: Add `arr[i]` to `current_sum`. This element is now inside the window.
        *   **Update `max_sum`**: Compare the `current_sum` with `max_sum` and update `max_sum` if `current_sum` is greater.

4.  **Return `max_sum`**: After the loop completes, `max_sum` will hold the maximum number of fruits that can be gathered from any `k` consecutive trees.

This sliding window approach ensures that we process each element of the array a constant number of times (once when it enters a window, once when it leaves), leading to an optimal time complexity.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   Calculating `k` takes $O(1)$ time.
    *   The first loop to calculate the initial window sum runs `k` times. In the worst case, `k` can be up to `N`, so this is $O(N)$.
    *   The second loop, which slides the window, runs `N - k` times. Each iteration involves constant-time operations (subtraction, addition, comparison). In the worst case, this is also $O(N)$.
    *   Combining these, the total time complexity is $O(k + (N - k)) = O(N)$, where `N` is the number of locations/trees in the `arr`.

*   **Space Complexity**: $O(1)$
    *   We use a few integer variables (`n`, `k`, `current_sum`, `max_sum`) to store intermediate values. These variables consume a constant amount of extra space, regardless of the input array size.
    *   No auxiliary data structures proportional to the input size are created.

## Solution Code

```cpp
#include <vector>    // Required for std::vector
#include <algorithm> // Required for std::min and std::max

class Solution {
public:
    // Function to find the maximum number of packages that can be delivered
    // within a given totalTime.
    // arr: A vector of integers where arr[i] is the number of packages at location i.
    // totalTime: The maximum time available for delivery.
    // Returns: The maximum number of packages delivered.
    int maxPackages(std::vector<int>& arr, int totalTime) {
        int n = arr.size(); // Get the total number of locations.

        // The number of consecutive locations the agent can visit is limited by:
        // 1. The total time available (`totalTime`).
        // 2. The total number of locations available (`n`).
        // So, the effective window size `k` is the minimum of these two values.
        // Constraints state 1 <= arr.size() and 1 <= totalTime,
        // which guarantees n >= 1 and totalTime >= 1, so k will always be >= 1.
        int k = std::min(totalTime, n);

        // `current_sum` will store the sum of packages in the current sliding window.
        // `max_sum` will store the maximum sum found across all possible windows.
        // The maximum possible sum (10^5 locations * 10^3 packages/location = 10^8)
        // fits within a standard 32-bit signed integer.
        int current_sum = 0;
        
        // Step 1: Calculate the sum of packages for the first window of size `k`.
        // This window covers locations from index 0 to k-1.
        for (int i = 0; i < k; ++i) {
            current_sum += arr[i];
        }

        // Initialize `max_sum` with the sum of the first window.
        int max_sum = current_sum;

        // Step 2: Slide the window across the rest of the array.
        // The loop starts from index `k`. This `i` represents the element
        // that is about to enter the window from the right.
        // The loop continues until `i` reaches `n`, meaning the last possible
        // window (ending at `n-1`) has been processed.
        for (int i = k; i < n; ++i) {
            // When the window slides one position to the right:
            // 1. The element at `i - k` (which is the leftmost element of the
            //    previous window) is no longer part of the window. Subtract its value.
            current_sum -= arr[i - k];
            
            // 2. The element at `i` (the new rightmost element) enters the window.
            //    Add its value to `current_sum`.
            current_sum += arr[i];
            
            // 3. Update `max_sum` if the `current_sum` is greater than the
            //    maximum sum found so far.
            max_sum = std::max(max_sum, current_sum);
        }

        // Return the maximum number of packages found across all valid windows.
        return max_sum;
    }
};
```