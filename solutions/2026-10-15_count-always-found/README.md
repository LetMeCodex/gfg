# Successful Binary Searches Irrespective of Pivot

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to count the number of elements in a given array `arr` that would be found by a binary search algorithm, regardless of the pivot selection strategy. Specifically, an element `arr[i]` is considered "always found" if, for any pivot chosen during a binary search for `arr[i]`, the search space will always contain `arr[i]` until it is eventually found.

## Intuition & Approach

The core idea behind binary search is to repeatedly divide the search interval in half. For an element `arr[i]` to be "always found," it must satisfy a specific condition relative to the elements to its left and right.

Consider an element `arr[i]`. If we are searching for `arr[i]` using binary search, and we pick a pivot `p`:
1. If `arr[i] < arr[p]`, the search continues in the left half.
2. If `arr[i] > arr[p]`, the search continues in the right half.
3. If `arr[i] == arr[p]`, the element is found.

For `arr[i]` to be *always* found, it must never be excluded from the search space prematurely. This means that `arr[i]` must be strictly greater than all elements to its left and strictly smaller than all elements to its right.

Let's break this down:
*   **Condition for elements to the left:** If `arr[i]` is to be found regardless of the pivot, it must be greater than any element `arr[j]` where `j < i`. If there was an element `arr[j]` to its left such that `arr[j] > arr[i]`, and the pivot chosen was `arr[j]` or an element to its right, the search would incorrectly discard the portion of the array containing `arr[i]`. Therefore, `arr[i]` must be greater than the maximum element in `arr[0...i-1]`.
*   **Condition for elements to the right:** Similarly, `arr[i]` must be smaller than any element `arr[k]` where `k > i`. If there was an element `arr[k]` to its right such that `arr[k] < arr[i]`, and the pivot chosen was `arr[k]` or an element to its left, the search would incorrectly discard the portion of the array containing `arr[i]`. Therefore, `arr[i]` must be smaller than the minimum element in `arr[i+1...n-1]`.

Combining these, an element `arr[i]` is "always found" if and only if:
`max(arr[0...i-1]) < arr[i] < min(arr[i+1...n-1])`

To efficiently check this condition for every element, we can precompute two auxiliary arrays:
1.  `prefixMax`: `prefixMax[i]` will store the maximum value in the subarray `arr[0...i]`. This allows us to find `max(arr[0...i-1])` in $O(1)$ time by looking at `prefixMax[i-1]`.
2.  `suffixMin`: `suffixMin[i]` will store the minimum value in the subarray `arr[i...n-1]`. This allows us to find `min(arr[i+1...n-1])` in $O(1)$ time by looking at `suffixMin[i+1]`.

**Handling Edge Cases:**
*   For `i = 0`, there are no elements to its left. We need a sentinel value for `max(arr[0...-1])`. Since the problem constraints usually imply positive integers (or at least non-negative), `0` is a safe sentinel, as `arr[i]` will always be greater than `0`. If negative numbers were possible, `INT_MIN` would be appropriate.
*   For `i = n-1`, there are no elements to its right. We need a sentinel value for `min(arr[n...n-1])`. Given the constraint that elements are up to $10^6$, `1000001` is a safe sentinel, as `arr[i]` will always be less than this. If larger numbers were possible, `INT_MAX` would be appropriate.

**Algorithm:**
1.  Initialize `prefixMax` array of size `n`.
    *   `prefixMax[0] = arr[0]`.
    *   For `i` from 1 to `n-1`, `prefixMax[i] = max(prefixMax[i-1], arr[i])`.
2.  Initialize `suffixMin` array of size `n`.
    *   `suffixMin[n-1] = arr[n-1]`.
    *   For `i` from `n-2` down to 0, `suffixMin[i] = min(suffixMin[i+1], arr[i])`.
3.  Initialize `count = 0`.
4.  Iterate through the array `arr` from `i = 0` to `n-1`:
    *   Determine `leftMax`: If `i == 0`, `leftMax = 0`. Otherwise, `leftMax = prefixMax[i-1]`.
    *   Determine `rightMin`: If `i == n-1`, `rightMin = 1000001`. Otherwise, `rightMin = suffixMin[i+1]`.
    *   If `leftMax < arr[i]` and `arr[i] < rightMin`, increment `count`.
5.  Return `count`.

The solution code provided implements this exact logic. The function name was changed to `countAlwaysFound` to match the driver code on GeeksforGeeks.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   Calculating `prefixMax` takes $O(N)$ time.
    *   Calculating `suffixMin` takes $O(N)$ time.
    *   The final loop to count the elements takes $O(N)$ time.
    *   Therefore, the total time complexity is $O(N) + O(N) + O(N) = O(N)$.

*   **Space Complexity**: $O(N)$
    *   We use two auxiliary arrays, `prefixMax` and `suffixMin`, each of size $N$.
    *   Therefore, the space complexity is $O(N)$.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // Required for std::max and std::min

class Solution {
public:
    /**
     * @brief Counts the number of elements in an array that would be found by a binary search
     *        regardless of the pivot selection strategy.
     * 
     * An element arr[i] is considered "always found" if it is strictly greater than all elements
     * to its left and strictly smaller than all elements to its right. This ensures that no matter
     * which pivot is chosen during a binary search for arr[i], the search space will always contain arr[i]
     * until it is eventually found.
     * 
     * @param arr The input vector of integers.
     * @return The count of elements that are always found.
     */
    int countAlwaysFound(std::vector<int>& arr) {
        int n = arr.size();
        
        // Handle empty array case: if the array is empty, no elements can be found.
        if (n == 0) {
            return 0;
        }

        // prefixMax[i] stores the maximum element in arr[0...i].
        // This helps to find max(arr[0]...arr[i-1]) efficiently in O(1) time.
        std::vector<int> prefixMax(n);
        prefixMax[0] = arr[0];
        for (int i = 1; i < n; ++i) {
            prefixMax[i] = std::max(prefixMax[i-1], arr[i]);
        }

        // suffixMin[i] stores the minimum element in arr[i...n-1].
        // This helps to find min(arr[i+1]...arr[n-1]) efficiently in O(1) time.
        std::vector<int> suffixMin(n);
        suffixMin[n-1] = arr[n-1];
        for (int i = n - 2; i >= 0; --i) {
            suffixMin[i] = std::min(suffixMin[i+1], arr[i]);
        }

        int count = 0;
        // Iterate through each index i to check if arr[i] is a "successful" target.
        for (int i = 0; i < n; ++i) {
            // leftMax represents max(arr[0]...arr[i-1]).
            // If i is 0, there are no elements to its left. We use 0 as a sentinel
            // because all arr[k] >= 1 (from constraints), so arr[i] will always be greater than 0.
            // If negative numbers were possible, we'd need a sentinel like INT_MIN.
            int leftMax = (i == 0) ? 0 : prefixMax[i-1];

            // rightMin represents min(arr[i+1]...arr[n-1]).
            // If i is n-1, there are no elements to its right. We use 1000001 as a sentinel
            // because all arr[k] <= 10^6 (from constraints), so arr[i] will always be less than 1000001.
            // If numbers larger than 10^6 were possible, we'd need a sentinel like INT_MAX.
            int rightMin = (i == n - 1) ? 1000001 : suffixMin[i+1]; 

            // An element arr[i] is always found if it's strictly greater than all elements to its left
            // and strictly smaller than all elements to its right.
            // This ensures that no matter which pivot is chosen, the search range will always
            // contain arr[i] until it's found.
            if (leftMax < arr[i] && arr[i] < rightMin) {
                count++;
            }
        }

        return count;
    }
};
```