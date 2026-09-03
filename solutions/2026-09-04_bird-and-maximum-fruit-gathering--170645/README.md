# Bird and Max Fruit Gathering

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find the maximum number of fruits a bird can gather. The fruits are arranged on trees in a circular manner. The bird can visit at most `m` consecutive trees. We are given an array `arr` where `arr[i]` represents the number of fruits on the i-th tree.

## Intuition & Approach

The core of this problem is to find a subarray of fruits that the bird can collect, considering the circular nature of the trees and the constraint on the number of consecutive trees (`m`).

**Initial Thoughts & Challenges:**

1.  **Circular Array:** The circular nature means that a sequence of `m` trees could wrap around from the end of the array to the beginning. For example, if `arr = [1, 2, 3, 4, 5]` and `m = 3`, the bird could collect fruits from trees `[4, 5, 1]` or `[5, 1, 2]`.
2.  **Consecutive Trees:** The bird can only pick from `m` *consecutive* trees. This implies we are looking for a subarray of a certain length.

**Handling Circularity:**

A common technique to handle circular arrays is to duplicate the array. If the original array is `arr` of size `n`, we can create an extended array `extended_arr` of size `2*n` by concatenating `arr` with itself (`arr + arr`). Now, any contiguous subarray of length up to `n` in the original circular array can be found as a contiguous subarray of length up to `n` in the `extended_arr`. Since the bird can visit at most `m` trees, and `m` can be up to `n`, we are interested in subarrays of length up to `m` within the `extended_arr`.

**Finding the Maximum Sum Subarray of a Fixed/Bounded Length:**

After extending the array, the problem transforms into finding the maximum sum of a subarray of length at most `m` in the `extended_arr`.

A naive approach would be to iterate through all possible start points in the `extended_arr` and for each start point, consider all subarrays of length 1 to `m`. This would be $O(n \cdot m)$ or $O(n^2)$ if `m` is close to `n`.

A more efficient approach uses prefix sums and a sliding window (or a deque for optimization).

1.  **Prefix Sums:** Calculate the prefix sums of the `extended_arr`. Let `prefix_sum[i]` be the sum of elements from index 0 to `i-1` in `extended_arr`. The sum of a subarray from index `i` to `j-1` (inclusive) is `prefix_sum[j] - prefix_sum[i]`.

2.  **Sliding Window with Deque (Monotonic Queue):** We want to maximize `prefix_sum[j] - prefix_sum[i]` such that `j - i <= m`. This is equivalent to maximizing `prefix_sum[j] - min(prefix_sum[i])` where `i` is in the range `[j-m, j-1]`.

    We can use a deque to efficiently find the minimum `prefix_sum[i]` within the sliding window of size `m`. The deque will store indices `i` such that `prefix_sum[i]` is in increasing order.

    *   **Iteration:** We iterate with `j` from 0 to `2*n`. `j` represents the right boundary of our potential subarray (exclusive).
    *   **Window Maintenance (Front):** Before considering `prefix_sum[j]`, we remove indices `i` from the front of the deque that are no longer within the window of size `m`. An index `i` is outside the window if `j - i > m`, which means `i < j - m`.
    *   **Calculate Max Sum:** If the deque is not empty, the index at the front of the deque (`dq.front()`) corresponds to the minimum `prefix_sum[i]` within the current valid window. We then calculate `prefix_sum[j] - prefix_sum[dq.front()]` and update `max_fruit` if this sum is greater.
    *   **Window Maintenance (Back):** To maintain the increasing order of prefix sums in the deque, we remove indices `k` from the back of the deque if `prefix_sum[k] >= prefix_sum[j]`. This is because if `prefix_sum[k] >= prefix_sum[j]` and `k < j`, then `k` will never be the optimal minimum for any future window ending at or after `j`, as `j` would be a better candidate (smaller or equal prefix sum and further to the right).
    *   **Add Current Index:** Finally, we add the current index `j` to the back of the deque.

**Edge Case:** If `m` is greater than or equal to `n`, the bird can visit all trees. In this case, the maximum fruits will be the sum of all fruits in the original array.

**Example Walkthrough:**
`arr = [1, 2, 3]`, `m = 2`

1.  `n = 3`, `m = 2`. `m < n`.
2.  `extended_arr = [1, 2, 3, 1, 2, 3]`
3.  `prefix_sum = [0, 1, 3, 6, 7, 9, 12]` (size `2*n + 1 = 7`)

    | j | Window for i (j-m to j-1) | dq (indices) | prefix_sum[j] | prefix_sum[dq.front()] | Current Sum (prefix_sum[j] - prefix_sum[dq.front()]) | max_fruit |
    |---|---------------------------|--------------|---------------|------------------------|------------------------------------------------------|-----------|
    | 0 | -                         | [0]          | 0             | -                      | -                                                    | 0         |
    | 1 | [ -1, 0 ]                 | [0, 1]       | 1             | 0                      | 1 - 0 = 1                                            | 1         |
    | 2 | [ 0, 1 ]                  | [0, 1, 2]    | 3             | 0                      | 3 - 0 = 3                                            | 3         |
    | 3 | [ 1, 2 ] (dq.front()=0 < 3-2=1, pop 0) | [1, 2, 3]    | 6             | 1                      | 6 - 1 = 5                                            | 5         |
    | 4 | [ 2, 3 ] (dq.front()=1 < 4-2=2, pop 1) | [2, 3, 4]    | 7             | 3                      | 7 - 3 = 4                                            | 5         |
    | 5 | [ 3, 4 ] (dq.front()=2 < 5-2=3, pop 2) | [3, 4, 5]    | 9             | 6                      | 9 - 6 = 3                                            | 5         |
    | 6 | [ 4, 5 ] (dq.front()=3 < 6-2=4, pop 3) | [4, 5, 6]    | 12            | 7                      | 12 - 7 = 5                                           | 5         |

    The maximum fruit collected is 5. This corresponds to the subarray `[3, 1, 2]` (indices 2, 3, 4 in `extended_arr`) which sums to 6, but the window size is `m=2`. The subarrays of length at most 2 are:
    `[1]` sum 1
    `[2]` sum 2
    `[3]` sum 3
    `[1]` sum 1
    `[2]` sum 2
    `[3]` sum 3
    `[1, 2]` sum 3
    `[2, 3]` sum 5
    `[3, 1]` sum 4
    `[1, 2]` sum 3
    `[2, 3]` sum 5
    The maximum is 5.

## Complexity Analysis

-   **Time Complexity**: $O(N)$, where $N$ is the size of the input array `arr`.
    -   Duplicating the array takes $O(N)$ time.
    -   Calculating prefix sums for the extended array (size $2N$) takes $O(N)$ time.
    -   The loop for the sliding window with the deque iterates through the extended array of size $2N$. Each element is added to and removed from the deque at most once. Therefore, this part takes $O(N)$ time.
    -   The overall time complexity is dominated by these linear operations, resulting in $O(N)$.

-   **Space Complexity**: $O(N)$.
    -   The `extended_arr` takes $O(N)$ space.
    -   The `prefix_sum` vector takes $O(N)$ space.
    -   The deque `dq` can store up to $O(N)$ indices in the worst case.
    -   Therefore, the overall space complexity is $O(N)$.

## Solution Code

```cpp
#include <vector>
#include <numeric>
#include <algorithm>
#include <deque>

class Solution {
public:
    /**
     * @brief Calculates the maximum number of fruits a bird can gather from trees arranged circularly.
     *
     * The bird can visit at most 'm' consecutive trees.
     *
     * @param arr A vector of integers representing the number of fruits on each tree.
     * @param m The maximum number of consecutive trees the bird can visit.
     * @return The maximum number of fruits the bird can gather.
     */
    long long maxFruits(std::vector<int>& arr, int m) {
        int n = arr.size();
        if (n == 0) {
            return 0;
        }

        // If m is greater than or equal to n, the bird can visit all trees.
        // The maximum fruits will be the sum of all fruits.
        if (m >= n) {
            long long total_sum = 0;
            for (int fruit : arr) {
                total_sum += fruit;
            }
            return total_sum;
        }

        // To handle the circularity of the trees, we create an extended array
        // by concatenating the original array with itself. This allows us to
        // treat any circular subarray of length up to 'n' as a linear subarray
        // within the extended array.
        std::vector<int> extended_arr = arr;
        for (int i = 0; i < n; ++i) {
            extended_arr.push_back(arr[i]);
        }

        // Calculate prefix sums for the extended array.
        // prefix_sum[i] will store the sum of elements from index 0 to i-1 in extended_arr.
        // This allows us to calculate the sum of any subarray arr[i...j-1] as prefix_sum[j] - prefix_sum[i].
        std::vector<long long> prefix_sum(extended_arr.size() + 1, 0);
        for (int i = 0; i < extended_arr.size(); ++i) {
            prefix_sum[i + 1] = prefix_sum[i] + extended_arr[i];
        }

        long long max_fruit = 0;
        // A deque (double-ended queue) to store indices for the prefix_sum array.
        // It will maintain indices in increasing order of their corresponding prefix_sum values,
        // effectively acting as a sliding window minimum queue for prefix sums.
        std::deque<int> dq;

        // Iterate through the extended array to find the maximum sum subarray of length at most m.
        // We are looking for max(prefix_sum[j] - prefix_sum[i]) where j - i <= m.
        // This is equivalent to max(prefix_sum[j] - min(prefix_sum[i])) for i in [j-m, j-1].
        // The loop for j goes up to 2*n because the subarray can wrap around.
        // j represents the right boundary (exclusive) of the subarray.
        for (int j = 0; j <= extended_arr.size(); ++j) {
            // 1. Remove indices from the front of the deque that are outside the window of size m.
            // The window for prefix_sum[i] is [j-m, j-1]. So, we need i >= j-m.
            // If dq.front() < j - m, it means the index is too far to the left and is outside the current window.
            if (!dq.empty() && dq.front() < j - m) {
                dq.pop_front();
            }

            // 2. Calculate the current maximum fruit if the deque is not empty.
            // The minimum prefix sum in the valid window is at dq.front().
            // The current sum of fruits for a subarray ending at index j-1 is prefix_sum[j] - prefix_sum[dq.front()].
            if (!dq.empty()) {
                max_fruit = std::max(max_fruit, prefix_sum[j] - prefix_sum[dq.front()]);
            }

            // 3. Maintain the increasing order of prefix sums in the deque.
            // Remove indices from the back whose prefix sums are greater than or equal to prefix_sum[j].
            // This ensures that dq.front() always points to the minimum prefix sum within the relevant window.
            // If prefix_sum[dq.back()] >= prefix_sum[j], then dq.back() will never be the optimal minimum
            // for any future window ending at or after 'j', because 'j' offers a smaller or equal prefix sum
            // and is further to the right.
            while (!dq.empty() && prefix_sum[dq.back()] >= prefix_sum[j]) {
                dq.pop_back();
            }

            // 4. Add the current index j to the deque.
            dq.push_back(j);
        }

        return max_fruit;
    }
};
```