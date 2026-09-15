# [Dominant Pairs](https://www.geeksforgeeks.org/problems/dominant-pairs/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 3 attempt(s)

## Problem Summary
Given an array `arr` of size `n` (where `n` is even), we need to find the number of "dominant pairs" `(i, j)` such that:
1. `0 <= i < n/2`
2. `n/2 <= j < n`
3. `arr[i] >= 5 * arr[j]`

## Intuition & Approach
The problem asks us to compare elements from the first half of the array with elements from the second half. A brute-force approach would involve nested loops, resulting in $O(n^2)$ time complexity, which might be inefficient for larger inputs.

To optimize this:
1. **Divide and Sort**: We isolate the second half of the array (`arr[n/2 ... n-1]`) and sort it. Sorting allows us to use binary search to quickly count how many elements satisfy the condition.
2. **Binary Search**: For each element `arr[i]` in the first half, we need to find how many elements `arr[j]` in the second half satisfy `arr[j] <= arr[i] / 5`. 
3. **Efficiency**: By using `std::upper_bound` on the sorted second half, we can find the count of elements satisfying the condition in $O(\log(n/2))$ time for each element in the first half.

## Complexity Analysis
- **Time Complexity**: $O(n \log n)$
  - Sorting the second half takes $O(\frac{n}{2} \log \frac{n}{2})$.
  - Iterating through the first half takes $O(\frac{n}{2})$ and performing `upper_bound` takes $O(\log \frac{n}{2})$ per iteration.
  - Total: $O(n \log n)$.
- **Space Complexity**: $O(n)$
  - We create a separate vector to store the second half of the array.

## Solution Code

```cpp
#include <vector>
#include <algorithm>

class Solution {
public:
    /**
     * @brief Counts the number of dominant pairs in an even-sized integer array.
     *
     * A pair of indices (i, j) is dominant if:
     * 0 <= i < arr.size() / 2
     * arr.size() / 2 <= j < arr.size()
     * arr[i] >= 5 * arr[j]
     *
     * @param arr The input even-sized integer array.
     * @return The total number of dominant pairs.
     */
    int dominantPairs(std::vector<int>& arr) {
        int n = arr.size();
        int mid = n / 2;
        int count = 0;

        // Extract the second half and sort it
        std::vector<int> second_half;
        second_half.reserve(n - mid);
        for (int j = mid; j < n; ++j) {
            second_half.push_back(arr[j]);
        }
        std::sort(second_half.begin(), second_half.end());

        // Iterate through the first half
        for (int i = 0; i < mid; ++i) {
            // The condition is arr[i] >= 5 * arr[j]
            // Which is equivalent to arr[j] <= arr[i] / 5.0
            // We use double to handle division correctly.
            double threshold = (double)arr[i] / 5.0;

            // Find the number of elements in second_half that are <= threshold.
            // upper_bound returns the first element > threshold.
            auto it = std::upper_bound(second_half.begin(), second_half.end(), threshold);
            
            count += (int)std::distance(second_half.begin(), it);
        }

        return count;
    }
};
```