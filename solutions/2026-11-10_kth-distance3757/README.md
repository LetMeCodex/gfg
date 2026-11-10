Here's a high-quality GitHub solution writeup for the given GeeksforGeeks problem:

---

# [Any Duplicate Within K Distance](https://www.geeksforg eeks.org/problems/kth-distance3757/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to determine if there exist two distinct indices `i` and `j` in a given integer array `arr` such that `arr[i]` is equal to `arr[j]` and the absolute difference between their indices, `abs(i - j)`, is at most `k`. If such a pair exists, we should return `true`; otherwise, return `false`.

**Example:**
`arr = [1, 2, 3, 1]`, `k = 3`
Output: `true` (because `arr[0] == arr[3] == 1` and `abs(0 - 3) = 3 <= k`)

`arr = [1, 0, 1, 1]`, `k = 1`
Output: `true` (because `arr[0] == arr[2] == 1` and `abs(0 - 2) = 2 > k`, but `arr[2] == arr[3] == 1` and `abs(2 - 3) = 1 <= k`)

## Intuition & Approach

The core idea is to efficiently keep track of the *last seen index* for each number encountered as we iterate through the array. When we encounter a number, we check if we've seen it before. If we have, we can immediately calculate the distance to its previous occurrence.

Here's the step-by-step approach:

1.  **Data Structure**: We'll use an `std::unordered_map<int, int>` (hash map) to store `(number, last_seen_index)` pairs. This allows for average O(1) lookup and insertion.

2.  **Iteration**: We iterate through the input array `arr` from left to right, using an index `i`.

3.  **Check for Duplicates**: For each element `arr[i]`:
    *   **If `arr[i]` is already in our map**: This means we've encountered this number before.
        *   Retrieve its `last_seen_index` from the map.
        *   Calculate the difference: `current_index (i) - last_seen_index`.
        *   If this difference is less than or equal to `k` (`i - last_seen_index <= k`), we have found a duplicate within the allowed distance. We can immediately return `true`.
    *   **If `arr[i]` is not in our map (or if it was but the distance was > k)**: We need to update its `last_seen_index`.
        *   Store/update `arr[i]` in the map with its current index `i` as its `last_seen_index`. This is crucial: even if a previous occurrence of `arr[i]` was too far away, this new `i` might be within `k` distance of a *future* occurrence. By always storing the *most recent* index, we ensure we're checking the smallest possible distance for any subsequent duplicates.

4.  **No Duplicates Found**: If we complete the loop without returning `true`, it means no such pair of duplicates within `k` distance exists in the entire array. In this case, we return `false`.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   We iterate through the array `arr` once, performing `N` operations.
    *   Inside the loop, `unordered_map` operations (`count`, `[]` access/insertion) take average O(1) time. In the worst-case scenario (due to hash collisions), these operations could degrade to O(N), but for typical inputs and good hash functions, the average case dominates.
    *   Therefore, the overall time complexity is **O(N)** on average.

*   **Space Complexity**: $O(N)$
    *   In the worst case, all elements in the array `arr` are distinct. In this scenario, the `unordered_map` will store `N` unique entries.
    *   Each entry stores an integer and an index.
    *   Therefore, the space complexity is **O(N)**.

## Solution Code

```cpp
#include <vector>
#include <unordered_map> // Required for std::unordered_map

class Solution {
public:
    /**
     * Checks if there exist two distinct indices i and j such that arr[i] is equal to arr[j]
     * and the absolute difference between i and j is at most k.
     *
     * @param arr The input array of integers.
     * @param k The maximum allowed distance between indices.
     * @return True if such a pair of indices exists, false otherwise.
     */
    bool checkWithinK(std::vector<int>& arr, int k) { // Renamed function from containsNearbyDuplicate to checkWithinK
        // num_to_index will store (number, last_seen_index) pairs.
        std::unordered_map<int, int> num_to_index;

        // Iterate through the array with current index 'i'.
        for (int i = 0; i < arr.size(); ++i) {
            // If the current number arr[i] is already in the map,
            // it means we have seen this number before.
            if (num_to_index.count(arr[i])) {
                // Check if the difference between the current index 'i' and
                // the previously seen index (stored in the map) is less than or equal to k.
                if (i - num_to_index[arr[i]] <= k) {
                    // If it is, we found a duplicate within k distance, so return true.
                    return true;
                }
            }
            // Always update the last seen index for the current number arr[i] to 'i'.
            // This is crucial:
            // 1. If arr[i] was not in the map, we add it.
            // 2. If arr[i] was in the map, but its previous occurrence was > k distance away,
            //    we update its index to 'i'. This new 'i' might form a valid pair
            //    with a future occurrence of arr[i].
            num_to_index[arr[i]] = i;
        }

        // If the loop completes without finding any such pair, return false.
        return false;
    }
};

```