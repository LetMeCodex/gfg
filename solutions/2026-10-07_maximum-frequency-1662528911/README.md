# [Maximum Frequency with K Increments](https://www.geeksforgeeks.org/problems/maximum-frequency-1662528911/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 1 attempt(s)

## Problem Summary

Given an array of integers `arr` and an integer `k`, we can increment any element of the array by 1 at most `k` times. The goal is to find the maximum possible frequency of any element in the array after performing these operations. In other words, we want to find the largest subarray where all elements can be made equal to the largest element in that subarray using at most `k` increments.

## Intuition & Approach

The core idea is to transform a subarray into a subarray where all elements are equal to the largest element within that subarray. If we can achieve this for a subarray of length `L` using at most `k` operations, then `L` is a potential candidate for the maximum frequency.

1.  **Sorting is Key**: If we consider a subarray `arr[left...right]`, and we want to make all elements equal to some value `X`, the most efficient way to do this is to make them all equal to `arr[right]` (the largest element in the sorted subarray). This is because incrementing is cheaper than decrementing (which isn't allowed here). If we sort the array first, then for any subarray `arr[left...right]`, the optimal target value to make all elements equal to is `arr[right]`. The total number of increments required would be the sum of differences: `(arr[right] - arr[left]) + (arr[right] - arr[left+1]) + ... + (arr[right] - arr[right-1])`. This can be rewritten as `(right - left + 1) * arr[right] - (arr[left] + arr[left+1] + ... + arr[right])`.

2.  **Sliding Window**: After sorting, we can use a sliding window approach. We maintain a window `[left, right]`.
    *   The `right` pointer expands the window by including a new element `arr[right]`.
    *   We calculate the sum of elements within the current window (`current_sum`).
    *   We then calculate the "cost" to make all elements in the window `[left, right]` equal to `arr[right]`. This cost is `(window_length * arr[right]) - current_sum`.
    *   If this `cost` is greater than `k`, it means we cannot make all elements in the current window equal to `arr[right]` with the allowed operations. To fix this, we need to shrink the window from the left. We remove `arr[left]` from `current_sum` and increment `left`. We repeat this shrinking process until the `cost` is less than or equal to `k`.
    *   Once the `cost` is within the limit, the current window `[left, right]` represents a valid subarray where all elements can be made equal to `arr[right]` using at most `k` operations. The length of this window (`right - left + 1`) is a potential maximum frequency. We update our `max_frequency` with the maximum length found so far.

3.  **Data Types**: It's important to use `long long` for `current_sum` and `cost` calculations. The sum of elements can exceed the capacity of a 32-bit integer, especially if the array size and element values are large. For example, if `n = 10^5` and `arr[i] = 10^6`, the sum can be up to `10^11`.

## Complexity Analysis

-   **Time Complexity**: $O(N \log N + N)$ which simplifies to $O(N \log N)$.
    *   Sorting the array takes $O(N \log N)$ time.
    *   The sliding window part involves two pointers, `left` and `right`. Both pointers traverse the array at most once. The `right` pointer moves from $0$ to $N-1$. The `left` pointer also moves from $0$ up to $N-1$. Each element is added to `current_sum` once and removed from `current_sum` at most once. Therefore, the sliding window operations take $O(N)$ time.
    *   The dominant factor is the sorting step.

-   **Space Complexity**: $O(1)$ (or $O(\log N)$ or $O(N)$ depending on the sorting algorithm's implementation).
    *   If the sorting is done in-place, the space complexity is $O(1)$ (excluding the input array).
    *   Some sorting algorithms might use auxiliary space, like $O(\log N)$ for quicksort's recursion stack or $O(N)$ for merge sort. However, for typical competitive programming scenarios, we often consider the space used by the algorithm itself, which can be considered constant if in-place sorting is used.

## Solution Code

```cpp
#include <vector>
#include <algorithm> // Required for std::sort and std::max

class Solution {
public:
    // Function to find the maximum possible frequency of any element
    // after performing at most k operations.
    // arr: The input integer array.
    // k: The maximum number of increment operations allowed.
    int maxFrequency(std::vector<int>& arr, int k) {
        // Step 1: Sort the array.
        // Sorting is crucial because if we want to make a subarray
        // [arr[left], ..., arr[right]] all equal to some value,
        // the optimal target value is arr[right] (the largest element in the window).
        // This minimizes the total increments needed.
        std::sort(arr.begin(), arr.end());

        int n = arr.size();
        long long current_sum = 0; // Stores the sum of elements in the current window [left, right].
                                   // Use long long to prevent potential overflow, as sum can be up to 10^5 * 10^6 = 10^11.
        int max_frequency = 0;     // Stores the maximum frequency found so far.
        int left = 0;              // Left pointer of the sliding window.

        // Step 2: Use a sliding window approach.
        // The 'right' pointer expands the window.
        for (int right = 0; right < n; ++right) {
            current_sum += arr[right]; // Add the current element to the window sum.

            // Calculate the cost to make all elements in the current window [left, right]
            // equal to arr[right].
            // The cost is (number_of_elements * target_value) - sum_of_elements.
            // Here, number_of_elements is (right - left + 1), and target_value is arr[right].
            long long window_length = right - left + 1;
            long long cost = window_length * arr[right] - current_sum;

            // If the calculated cost exceeds the allowed operations 'k',
            // we need to shrink the window from the left.
            // We remove elements from the left until the cost condition is met.
            while (cost > k) {
                current_sum -= arr[left]; // Remove arr[left] from the sum.
                left++;                   // Move the left pointer to the right.
                
                // Recalculate window_length and cost for the new, smaller window.
                // The target value (arr[right]) remains the same as we are only shrinking from left.
                window_length = right - left + 1;
                cost = window_length * arr[right] - current_sum;
            }

            // At this point, the current window [left, right] is valid, meaning
            // all its elements can be made equal to arr[right] with at most 'k' operations.
            // Update the maximum frequency found.
            // window_length is at most N (10^5), so it fits in an int.
            max_frequency = std::max(max_frequency, (int)window_length);
        }

        // The problem guarantees arr.size() >= 1, so max_frequency will be at least 1.
        return max_frequency;
    }
};
```