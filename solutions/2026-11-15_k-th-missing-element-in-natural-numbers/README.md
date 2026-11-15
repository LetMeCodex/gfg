# [K-th Missing in Natural Numbers](https://www.geeksforgeeks.org/problems/k-th-missing-element-in-natural-numbers/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

Given a sorted array `arr` of distinct positive integers and an integer `k`, find the `k`-th positive integer that is missing from this array.

## Intuition & Approach

The core idea is to count the missing numbers and identify where the `k`-th missing number falls. Since the array contains distinct positive integers and is sorted, we can iterate through it and calculate the number of missing integers between consecutive elements.

1.  **Initial Missing Numbers**: Before the first element `arr[0]`, the natural numbers are `1, 2, ..., arr[0]-1`. The count of missing numbers before `arr[0]` is `arr[0] - 1`. If `k` is less than or equal to this count, then the `k`-th missing number is simply `k`.

2.  **Iterating Through Gaps**: If `k` is greater than the missing numbers before `arr[0]`, we subtract these initial missing numbers from `k`. Then, we iterate through the array from the first element up to the second-to-last element. For each pair of adjacent elements `arr[i]` and `arr[i+1]`, the number of missing integers between them is `arr[i+1] - arr[i] - 1`.
    *   If `k` is less than or equal to this `missing_count`, it means the `k`-th missing number falls within this gap. The `k`-th missing number will be `arr[i] + k`.
    *   If `k` is greater than `missing_count`, we subtract `missing_count` from `k` and continue to the next gap.

3.  **Missing Numbers Beyond the Array**: If we iterate through all the gaps in the array and `k` is still greater than 0, it implies that the `k`-th missing number is beyond the last element of the array. In this case, the `k`-th missing number will be `arr.back() + k`.

**Example Walkthrough:**
Let `arr = [2, 3, 4, 7, 11]` and `k = 5`.

1.  **Before `arr[0]` (2)**: Missing numbers are `1`. Count = `2 - 1 = 1`.
    Since `k = 5` is not `<= 1`, we update `k = 5 - 1 = 4`.

2.  **Gap between `arr[0]` (2) and `arr[1]` (3)**: Missing numbers = `3 - 2 - 1 = 0`.
    `k = 4` is not `<= 0`. Update `k = 4 - 0 = 4`.

3.  **Gap between `arr[1]` (3) and `arr[2]` (4)**: Missing numbers = `4 - 3 - 1 = 0`.
    `k = 4` is not `<= 0`. Update `k = 4 - 0 = 4`.

4.  **Gap between `arr[2]` (4) and `arr[3]` (7)**: Missing numbers = `7 - 4 - 1 = 2`. (These are 5, 6).
    `k = 4` is not `<= 2`. Update `k = 4 - 2 = 2`.

5.  **Gap between `arr[3]` (7) and `arr[4]` (11)**: Missing numbers = `11 - 7 - 1 = 3`. (These are 8, 9, 10).
    `k = 2` is `<= 3`. The 2nd missing number in this gap is `arr[3] + k = 7 + 2 = 9`.
    So, the 5th missing number is 9.

The code implements this logic directly. The initial sorting is important because the problem statement guarantees distinct positive integers, but not necessarily sorted input. The driver code in GeeksforGeeks might pass a sorted array, but it's good practice to sort if the problem statement doesn't explicitly guarantee it. The function name `findKthSmall` is used to match the driver code's expectation.

## Complexity Analysis

-   **Time Complexity**: $O(N \log N)$ due to the sorting step, where $N$ is the number of elements in the input array `arr`. If the input array is guaranteed to be sorted, the time complexity would be $O(N)$ for the linear scan.
-   **Space Complexity**: $O(1)$ if the sorting is done in-place. If a copy of the array is made for sorting (depending on the `std::sort` implementation or if we choose to copy), it could be $O(N)$. However, typically `std::sort` modifies the input vector directly, leading to $O(1)$ auxiliary space.

## Solution Code

```cpp
#include <vector>
#include <numeric>
#include <algorithm>

class Solution {
public:
    // Renamed from kthMissing to findKthSmall to match the driver code.
    int findKthSmall(std::vector<int>& arr, int k) {
        // Sort the array to easily calculate missing numbers.
        // Although the problem statement says distinct positive integers,
        // sorting is crucial for the logic below.
        std::sort(arr.begin(), arr.end());

        // If k is smaller than the first missing number before arr[0]
        // The number of missing elements before arr[0] is arr[0] - 1.
        if (k <= arr[0] - 1) {
            return k;
        }

        // Adjust k by subtracting the missing numbers before the first element.
        k -= (arr[0] - 1);

        // Iterate through the sorted array to find the k-th missing number.
        for (size_t i = 0; i < arr.size() - 1; ++i) {
            // Calculate the number of missing elements between arr[i] and arr[i+1].
            int missing_count = arr[i+1] - arr[i] - 1;

            // If k falls within this range of missing numbers.
            if (k <= missing_count) {
                // The k-th missing number is arr[i] + k.
                return arr[i] + k;
            }
            // Otherwise, subtract the missing numbers from k and continue.
            k -= missing_count;
        }

        // If k is still greater than 0 after checking all gaps in the array,
        // it means the k-th missing number is beyond the last element of arr.
        // The k-th missing number will be the last element of arr plus the remaining k.
        return arr.back() + k;
    }
};
```