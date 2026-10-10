# [Sort Two Parts Sorted](https://www.geeksforgeeks.org/problems/sort-the-half-sorted2157/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to sort an array `arr` of `N` elements that is "half-sorted". This means the array is composed of two sorted parts. Specifically, there exists an index `k` such that the subarray `arr[0...k]` is sorted in non-decreasing order, and the subarray `arr[k+1...N-1]` is also sorted in non-decreasing order. Our task is to sort the entire array `arr` in non-decreasing order.

**Example:**
If `arr = [10, 20, 30, 40, 50, 5, 15, 25, 35]`,
The first sorted part is `[10, 20, 30, 40, 50]` (from index 0 to 4).
The second sorted part is `[5, 15, 25, 35]` (from index 5 to 8).
The desired output is `[5, 10, 15, 20, 25, 30, 35, 40, 50]`.

## Intuition & Approach

The core idea behind solving this problem is to recognize that we are essentially given two sorted arrays that are concatenated. If we can identify the "breakpoint" where the first sorted part ends and the second begins, we can then use a standard merge operation (similar to the one in Merge Sort) to combine these two sorted parts into a single, fully sorted array.

Here's a step-by-step breakdown of the approach:

1.  **Handle Base Cases**: If the array has 0 or 1 element, it is already sorted. We can return immediately.

2.  **Find the Breakpoint**:
    *   The array `arr` is `[part1_sorted | part2_sorted]`.
    *   Since both `part1_sorted` and `part2_sorted` are individually sorted, the only place where the overall sorted property might be violated is at the boundary between them.
    *   We iterate through the array from `i = 0` to `N-2`. If we find an index `i` such that `arr[i] > arr[i+1]`, then `i+1` is the start of the second sorted part (our `breakpoint_idx`). This is because `arr[i]` is the last element of the first part, and `arr[i+1]` is the first element of the second part, and this condition indicates that the second part starts with a value smaller than the end of the first part.
    *   If no such `i` is found after iterating through the entire array, it means the array is already fully sorted (e.g., `[1, 2, 3, 4, 5]`). In this scenario, `breakpoint_idx` will remain at its initial sentinel value (e.g., -1), and we can return immediately as no merging is needed.

3.  **Merge the Two Sorted Parts**:
    *   Once `breakpoint_idx` is identified (and it's not -1), we have two distinct sorted subarrays:
        *   `Part 1`: `arr[0 ... breakpoint_idx - 1]`
        *   `Part 2`: `arr[breakpoint_idx ... N - 1]`
    *   We'll use a temporary array, `temp_arr`, of size `N` to store the merged result.
    *   Initialize three pointers:
        *   `p1 = 0` (points to the start of `Part 1`)
        *   `p2 = breakpoint_idx` (points to the start of `Part 2`)
        *   `k = 0` (points to the current insertion position in `temp_arr`)
    *   While both `p1` is within `Part 1`'s bounds and `p2` is within `Part 2`'s bounds:
        *   Compare `arr[p1]` and `arr[p2]`.
        *   Copy the smaller element to `temp_arr[k]`, then increment the corresponding pointer (`p1` or `p2`) and `k`.
    *   After one of the parts is exhausted, copy any remaining elements from the other part into `temp_arr`. This handles cases where one part is longer than the other.
    *   Finally, copy all elements from `temp_arr` back to the original `arr`.

This approach effectively transforms the problem into a classic merge operation after a quick initial scan to find the split point.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   **Finding the breakpoint**: We iterate through the array once in the worst case to find the breakpoint. This takes $O(N)$ time.
    *   **Merging the two parts**: We iterate through both parts using pointers, comparing and copying elements to a temporary array. This process involves visiting each element of the array at most a constant number of times, taking $O(N)$ time.
    *   **Copying back**: Copying elements from the temporary array back to the original array takes another $O(N)$ time.
    *   Combining these steps, the total time complexity is $O(N) + O(N) + O(N) = O(N)$.

*   **Space Complexity**: $O(N)$
    *   We use a temporary `std::vector<int> temp_arr` of size `N` to store the merged result. This contributes $O(N)$ to the space complexity.
    *   A few integer pointers (`p1`, `p2`, `k`, `breakpoint_idx`, `n`) use $O(1)$ auxiliary space.
    *   Therefore, the dominant factor is the temporary array, making the total space complexity $O(N)$.

## Solution Code

```cpp
#include <vector> // Required for std::vector

class Solution {
public:
    // Renamed the method from 'sortTwoPartsSorted' to 'mergeTwoParts'
    // to match the name expected by the GeeksforGeeks driver code.
    void mergeTwoParts(std::vector<int>& arr) {
        int n = arr.size();

        // Handle base cases: An array with 0 or 1 element is already sorted.
        if (n <= 1) {
            return;
        }

        // Step 1: Find the breakpoint.
        // The breakpoint is the index where the second sorted part begins.
        // This is the first index 'i' such that arr[i-1] > arr[i].
        // If the array is already fully sorted, no such 'i' will be found,
        // and breakpoint_idx will remain -1.
        int breakpoint_idx = -1;
        for (int i = 0; i < n - 1; ++i) {
            if (arr[i] > arr[i+1]) {
                breakpoint_idx = i + 1;
                break; // Found the breakpoint, exit loop
            }
        }

        // Step 2: If no breakpoint was found, the array is already sorted.
        // In this case, we don't need to perform any merge operation.
        if (breakpoint_idx == -1) {
            return;
        }

        // Step 3: Merge the two sorted parts.
        // Part 1: arr[0 ... breakpoint_idx - 1]
        // Part 2: arr[breakpoint_idx ... n - 1]
        
        // Create a temporary array to store the merged result.
        // This uses O(N) auxiliary space.
        std::vector<int> temp_arr(n);
        
        // Pointers for the two parts and the temporary array.
        int p1 = 0; // Pointer for the first part (starts at index 0)
        int p2 = breakpoint_idx; // Pointer for the second part (starts at breakpoint_idx)
        int k = 0; // Pointer for the temporary array (starts at index 0)

        // Merge elements from both parts into temp_arr until one part is exhausted.
        while (p1 < breakpoint_idx && p2 < n) {
            if (arr[p1] <= arr[p2]) {
                temp_arr[k++] = arr[p1++];
            } else {
                temp_arr[k++] = arr[p2++];
            }
        }

        // Copy any remaining elements from the first part (if any).
        // This loop will run only if p1 has not reached breakpoint_idx,
        // meaning p2's part was exhausted first.
        while (p1 < breakpoint_idx) {
            temp_arr[k++] = arr[p1++];
        }

        // Copy any remaining elements from the second part (if any).
        // This loop will run only if p2 has not reached n,
        // meaning p1's part was exhausted first.
        while (p2 < n) {
            temp_arr[k++] = arr[p2++];
        }

        // Step 4: Copy the sorted elements from temp_arr back to the original arr.
        for (int i = 0; i < n; ++i) {
            arr[i] = temp_arr[i];
        }
    }
};
```