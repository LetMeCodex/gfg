Here's a high-quality GitHub solution writeup for the GeeksforGeeks POTD: "At Least K Occurrences".

---

# [At Least K Occurrences](https://www.geeksforgreeks.org/problems/first-element-to-occur-k-times5150/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find the "first element to occur K times" in a given array `arr`. This means we need to identify the element whose `K`-th occurrence appears earliest in the array. If multiple elements reach their `K`-th occurrence at the same earliest index, the problem implies we should return the one that was encountered first during the left-to-right traversal. If no element appears `K` or more times, we should return `-1`.

**Example:**
`arr = [1, 7, 4, 3, 4, 8, 7]`, `K = 2`

*   `1`: Appears once.
*   `7`: First occurrence at index 1, second at index 6.
*   `4`: First occurrence at index 2, second at index 4.
*   `3`: Appears once.
*   `8`: Appears once.

Elements that occur at least `K=2` times are `7` and `4`.
*   `7`'s 2nd occurrence is at index 6.
*   `4`'s 2nd occurrence is at index 4.

Comparing the indices of their `K`-th occurrences, `4`'s 2nd occurrence (index 4) is earlier than `7`'s (index 6). Therefore, the answer is `4`.

## Intuition & Approach

**Intuition:**
To determine if an element has occurred `K` times, we need to keep track of the frequency of each number encountered so far. Since we are looking for the *earliest* `K`-th occurrence, we must process the array elements in their given order (from left to right). The moment an element's count reaches `K`, we know its `K`-th occurrence has just happened at the current index. We need to store the element and this index, and continuously update our "best candidate" if we find an element whose `K`-th occurrence happens even earlier.

**Approach:**

1.  **Frequency Counter:** We need an efficient way to store and update the counts of each element. Given the constraints (`0 <= arr[i] <= 10^6`), an `std::unordered_map<int, int>` is an excellent choice. It provides average `O(1)` time complexity for insertions and lookups, and handles potentially sparse data (where numbers might be large but not all numbers in the range appear) efficiently. A `std::vector<int>` of size `10^6 + 1` could also work for these specific constraints, potentially offering slightly better cache performance, but `unordered_map` is more general for broader value ranges.

2.  **Tracking the Earliest `K`-th Occurrence:**
    *   Initialize `result_element` to `-1` (to be returned if no element meets the criteria).
    *   Initialize `min_kth_occurrence_index` to `arr.size()`. This value is greater than any valid index in the array (`0` to `arr.size() - 1`), serving as an initial "infinity" for finding the minimum index.

3.  **Iterate and Update:**
    *   Iterate through the array `arr` from `i = 0` to `arr.size() - 1`.
    *   For each `current_element = arr[i]`:
        *   Increment its count in the `counts` map: `counts[current_element]++`.
        *   **Crucially**, check if `counts[current_element]` has *just* reached `K`. We only care about the first time an element's count hits `K`; subsequent occurrences (where `counts[current_element] > K`) do not change the index of its `K`-th occurrence.
        *   If `counts[current_element] == K`:
            *   This means `current_element` has achieved its `K`-th occurrence at the current index `i`.
            *   Compare `i` with `min_kth_occurrence_index`.
            *   If `i` is less than `min_kth_occurrence_index`, it means we've found an element whose `K`-th occurrence is earlier than any previously found candidate. Update `min_kth_occurrence_index = i` and `result_element = current_element`.

4.  **Return Result:** After the loop finishes, `result_element` will hold the element whose `K`-th occurrence was the earliest. If `min_kth_occurrence_index` is still `arr.size()`, it implies no element ever reached `K` occurrences, so we return `-1`.

**Addressing "Solved in: 2 attempt(s)":**
The initial attempt might have faced issues such as a simple method name mismatch (as hinted by the comments in the provided code, where the driver expected `firstElementKTime` but a different name might have been used), or potentially an incorrect logic for tracking the minimum index or handling the edge case where no element occurs `K` times. The refined approach ensures correct tracking of counts and the earliest `K`-th occurrence index.

## Complexity Analysis

*   **Time Complexity**:
    *   The solution involves a single pass through the input array `arr`, which takes `O(N)` time, where `N` is the size of the array.
    *   Inside the loop, `unordered_map` operations (insertion, lookup, and increment) are performed. On average, these operations take `O(1)` time. In the worst-case scenario (due to hash collisions), they could degrade to `O(M)` where `M` is the number of unique elements, but this is rare with good hash functions.
    *   Therefore, the overall average time complexity is **$O(N)$**.

*   **Space Complexity**:
    *   We use an `std::unordered_map` (`counts`) to store the frequencies of elements. In the worst case, all elements in the array are unique. In this scenario, the map will store `N` entries.
    *   Thus, the space complexity is **$O(M)$**, where `M` is the number of unique elements in the array. In the worst case, `M` can be equal to `N`. Given `arr[i]` can be up to `10^6`, `M` can also be up to `10^6`.

## Solution Code

```cpp
#include <vector>
#include <unordered_map> // Using unordered_map for frequency counting

class Solution {
public:
    // The problem asks for a method named 'firstElementKTime'.
    int firstElementKTime(std::vector<int>& arr, int k) {
        // Constraints:
        // 1 <= arr.size() <= 10^6
        // 0 <= arr[i] <= 10^6
        // 1 <= k <= 10^3

        // We need to find the first element whose k-th occurrence appears earliest.
        // This implies tracking counts of each element.

        // Use an unordered_map to store the frequency of each element.
        // This is efficient for potentially sparse large values (arr[i] up to 10^6)
        // and provides O(1) average time complexity for lookups and insertions.
        std::unordered_map<int, int> counts;

        int result_element = -1; // Stores the element that is the answer.
        // Stores the index of the earliest k-th occurrence found so far.
        // Initialize with a value larger than any possible array index (0 to arr.size() - 1).
        // arr.size() is a suitable initial value.
        int min_kth_occurrence_index = arr.size();

        // Iterate through the array from left to right.
        for (int i = 0; i < arr.size(); ++i) {
            int current_element = arr[i];
            counts[current_element]++; // Increment count for the current element.

            // If the count for this element just reached k, it's a candidate.
            // We only care about the *first* time its count reaches k.
            // If its count goes beyond k, it doesn't change its k-th occurrence index.
            if (counts[current_element] == k) {
                // This element's k-th occurrence just happened at index 'i'.
                // Check if this occurrence is earlier than the current earliest k-th occurrence.
                if (i < min_kth_occurrence_index) {
                    min_kth_occurrence_index = i;      // Update the minimum index.
                    result_element = current_element;   // Update the result element.
                }
            }
        }

        // After iterating through the entire array, if min_kth_occurrence_index
        // is still arr.size(), it means no element reached k occurrences.
        // In this case, result_element will remain -1, which is the correct return value.
        return result_element;
    }
};
```