This writeup provides a detailed explanation of the solution for the "Two Pairs with Same Sum" problem on GeeksforGeeks.

---

# [Two Pairs with Same Sum](https://www.geeksforgreeks.org/problems/satisfy-the-equation5847/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to find four distinct indices `a, b, c, d` from a given array `arr` such that the sum of elements at indices `a` and `b` is equal to the sum of elements at indices `c` and `d`. Specifically, we need to satisfy the equation `arr[a] + arr[b] = arr[c] + arr[d]`.

The conditions for the indices are:
1.  `a < b` and `c < d` (indices within each pair must be in increasing order).
2.  All four indices `a, b, c, d` must be distinct. That is, `a != c`, `a != d`, `b != c`, and `b != d`.
3.  If multiple such quadruplets `(a, b, c, d)` exist, we must return the lexicographically smallest one. A crucial part of this lexicographical ordering is that `a < c` must also be satisfied for any valid quadruplet.

If no such quadruplet is found, we should return `{-1, -1, -1, -1}`.

## Intuition & Approach

The core idea is to find two distinct pairs of indices `(a, b)` and `(c, d)` that yield the same sum. This type of problem often lends itself to using a hash map (or `std::map` in C++) to store intermediate sums and their corresponding indices.

1.  **Iterate through all possible pairs:** We can iterate through all unique pairs `(i, j)` from the input array `arr` such that `i < j`. For each pair, we calculate their sum `current_sum = arr[i] + arr[j]`.

2.  **Store sums and their first encountered pair:** We use a `std::map<int, std::pair<int, int>>` (let's call it `sum_to_indices`).
    *   The `key` of the map will be the `current_sum`.
    *   The `value` will be a `std::pair<int, int>` storing the indices `(i, j)` of the *first* pair encountered that produced this `current_sum`.

3.  **Check for existing sums:**
    *   When we calculate `current_sum` for a pair `(i, j)`:
        *   **If `current_sum` is already in `sum_to_indices`**: This means we have found a previous pair `(prev_a, prev_b)` (stored in the map) that sums to the same `current_sum`. Now we have two potential pairs: `(prev_a, prev_b)` and `(i, j)`.
            *   **Distinctness Check**: We must ensure that all four indices `prev_a, prev_b, i, j` are distinct. The conditions `prev_a < prev_b` and `i < j` are guaranteed by how we populate the map and iterate through pairs. We only need to check `prev_a != i`, `prev_a != j`, `prev_b != i`, and `prev_b != j`.
            *   **Forming the Quadruplet and Lexicographical Order**: If all four indices are distinct, we have a valid candidate quadruplet. According to the problem statement, for a quadruplet `(a, b, c, d)` to be valid, `a < c` must hold. To satisfy this, we compare `prev_a` and `i`:
                *   If `prev_a < i`, then `(a, b) = (prev_a, prev_b)` and `(c, d) = (i, j)`. The candidate quadruplet is `{prev_a, prev_b, i, j}`.
                *   If `i < prev_a`, then `(a, b) = (i, j)` and `(c, d) = (prev_a, prev_b)`. The candidate quadruplet is `{i, j, prev_a, prev_b}`.
                (Note: `prev_a` cannot be equal to `i` because we already checked for distinctness).
            *   **Update Result**: We maintain a `result` vector, initialized to `{-1, -1, -1, -1}`. Whenever we find a valid `current_quadruplet`, we compare it with `result`. If `result` is still `{-1, -1, -1, -1}` (meaning no quadruplet found yet) or if `current_quadruplet` is lexicographically smaller than `result`, we update `result` with `current_quadruplet`. `std::vector`'s `operator<` naturally performs lexicographical comparison, which simplifies this step.
        *   **If `current_sum` is NOT in `sum_to_indices`**: This is the first time we've encountered this sum. We store the current pair `(i, j)` in the map: `sum_to_indices[current_sum] = {i, j}`.

4.  **Return Result**: After iterating through all possible pairs, the `result` vector will hold the lexicographically smallest quadruplet found, or `{-1, -1, -1, -1}` if no such quadruplet exists.

This approach systematically explores all possibilities while efficiently using the map to find matching sums and correctly handles the distinctness and lexicographical ordering requirements.

## Complexity Analysis

*   **Time Complexity**: $O(N^2 \log N)$
    *   The nested loops iterate through all possible pairs `(i, j)` such that `i < j`. There are approximately $N^2/2$ such pairs, leading to $O(N^2)$ iterations.
    *   Inside the loops, map operations (`count`, insertion, access) take $O(\log K)$ time, where `K` is the number of distinct sums stored in the map. In the worst case, `K` can be up to $O(N^2)$ (if all sums are distinct). Therefore, each map operation takes $O(\log N^2) = O(2 \log N) = O(\log N)$ time.
    *   Vector creation and comparison for fixed size 4 takes $O(1)$ time.
    *   Combining these, the total time complexity is $O(N^2 \log N)$.

*   **Space Complexity**: $O(N^2)$
    *   The `sum_to_indices` map stores at most $O(N^2)$ distinct sums. Each entry stores an integer key and a pair of integers as value.
    *   The `result` and `current_quadruplet` vectors store a fixed number of elements (4 integers), contributing $O(1)$ space.
    *   Therefore, the dominant space complexity comes from the map, resulting in $O(N^2)$.

## Solution Code

```cpp
#include <vector>
#include <map>
#include <algorithm> // Required for std::vector comparison (lexicographical)

class Solution {
public:
    std::vector<int> findPairs(std::vector<int>& arr) {
        int N = arr.size(); // Get the size of the input array.

        // A map to store sums of pairs and the indices of the first pair that produced that sum.
        // Key: The sum of two elements (arr[i] + arr[j]).
        // Value: A pair of integers {i, j} representing the indices of the first pair found.
        // Using std::map ensures elements are ordered by key, but for this problem,
        // the order of keys doesn't directly affect the lexicographical result,
        // only the order of iteration for i and j does. std::unordered_map could also be used
        // for potentially faster average-case performance, but std::map offers guaranteed O(log N)
        // worst-case for operations, which is sufficient here.
        std::map<int, std::pair<int, int>> sum_to_indices;

        // Initialize the result vector with {-1, -1, -1, -1},
        // which signifies that no such quadruplet has been found yet.
        std::vector<int> result = {-1, -1, -1, -1};

        // Iterate through all possible pairs (i, j) such that i < j.
        // This ensures that for any pair (i, j), i and j are distinct and i is always smaller than j.
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                int current_sum = arr[i] + arr[j]; // Calculate the sum of the current pair.

                // Check if this 'current_sum' has been encountered before.
                if (sum_to_indices.count(current_sum)) {
                    // If the sum exists in the map, it means we found a previous pair
                    // (prev_a, prev_b) that also sums to 'current_sum'.
                    int prev_a = sum_to_indices[current_sum].first;
                    int prev_b = sum_to_indices[current_sum].second;

                    // Now we have two pairs: (prev_a, prev_b) and (i, j).
                    // We need to ensure all four indices (prev_a, prev_b, i, j) are distinct.
                    // The conditions prev_a < prev_b and i < j are already guaranteed by
                    // how sum_to_indices is populated and the loop structure.
                    // We only need to check if prev_a, prev_b are distinct from i, j.
                    if (prev_a != i && prev_a != j && prev_b != i && prev_b != j) {
                        // All four indices are distinct. We have found a valid quadruplet.
                        std::vector<int> current_quadruplet;

                        // To satisfy the lexicographical requirement (a < c),
                        // we assign the pair with the smaller first index as (a, b)
                        // and the other pair as (c, d).
                        if (prev_a < i) {
                            current_quadruplet = {prev_a, prev_b, i, j};
                        } else { // i < prev_a (prev_a cannot be equal to i because of the distinctness check)
                            current_quadruplet = {i, j, prev_a, prev_b};
                        }

                        // Compare this newly found quadruplet with the current best 'result'.
                        // std::vector's operator< performs lexicographical comparison,
                        // which is exactly what's needed for the problem.
                        if (result[0] == -1 || current_quadruplet < result) {
                            result = current_quadruplet; // Update result if current_quadruplet is smaller.
                        }
                    }
                } else {
                    // If 'current_sum' is not in the map, this is the first time we've seen it.
                    // Store the current pair (i, j) as the representative for this sum.
                    sum_to_indices[current_sum] = {i, j};
                }
            }
        }

        // After checking all possible pairs, 'result' will hold the lexicographically
        // smallest quadruplet or {-1, -1, -1, -1} if none was found.
        return result;
    }
};
```