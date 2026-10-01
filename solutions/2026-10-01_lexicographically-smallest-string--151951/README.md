# [Lexicographically Smallest Rotation](https://www.geeksforgeeks.org/problems/lexicographically-smallest-string--151951/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Hard
- **Solved In**: 3 attempt(s)

## Problem Summary
Given a string `s`, the task is to find the lexicographically smallest string that can be obtained by rotating `s` any number of times to the left. A left rotation means moving the first character to the end of the string. For example, if `s = "abc"`, its rotations are "abc", "bca", and "cab". The lexicographically smallest among these is "abc".

## Intuition & Approach

The naive approach would be to generate all `N` possible rotations of the string `s`, store them, and then find the lexicographically smallest one. This would involve `N` string creations (each $O(N)$) and `N-1` string comparisons (each $O(N)$), leading to an overall time complexity of $O(N^2)$. For larger strings, this can be inefficient.

A more efficient approach, often referred to as a variant of Booth's algorithm or Duval's algorithm for finding the lexicographically smallest cyclic shift, can solve this problem in linear time, $O(N)$.

Here's the intuition behind the provided solution:

1.  **Double the String**: The key insight is to concatenate the string `s` with itself to form `s_double = s + s`. Any cyclic rotation of `s` will appear as a substring of `s_double` of length `N`. For example, if `s = "abcd"`, then `s_double = "abcdabcd"`. The rotations are:
    *   "abcd" (from `s_double[0...3]`)
    *   "bcda" (from `s_double[1...4]`)
    *   "cdab" (from `s_double[2...5]`)
    *   "dabc" (from `s_double[3...6]`)
    This trick allows us to compare any two rotations by simply comparing substrings of `s_double` without explicit rotation operations.

2.  **Two-Pointer Comparison (`i`, `j`, `k`)**:
    *   We maintain two pointers, `i` and `j`, representing the starting indices of two candidate lexicographically smallest rotations within `s_double`. Initially, `i = 0` and `j = 1`.
    *   `k` represents the length of the common prefix between the rotation starting at `i` and the rotation starting at `j`. That is, `s_double[i...i+k-1]` is identical to `s_double[j...j+k-1]`.

3.  **Iterative Comparison and Elimination**:
    *   The algorithm proceeds by comparing `s_double[i + k]` and `s_double[j + k]`.
    *   **If `s_double[i + k] == s_double[j + k]`**: The characters match, so the common prefix extends. We increment `k`.
    *   **If `s_double[i + k] < s_double[j + k]`**: This means the rotation starting at `i` is lexicographically smaller than the rotation starting at `j` (at least at this point). Crucially, any rotation starting from `j` up to `j+k` cannot be the smallest. Why? Because all these rotations would share a prefix with `s_double[j...]` that is identical to `s_double[i...]` up to `k` characters, but then `s_double[j+k]` is greater than `s_double[i+k]`. Therefore, we can eliminate `j` and all positions between `j` and `j+k` as potential candidates. The next valid candidate for `j` must be `j + k + 1`. We then reset `k` to `0` because the new `j` has no guaranteed common prefix with `i`.
    *   **If `s_double[i + k] > s_double[j + k]`**: Symmetrically, the rotation starting at `j` is smaller. We eliminate `i` and all positions between `i` and `i+k`, setting `i = i + k + 1` and resetting `k = 0`.

4.  **Handling `i == j`**: If `i` and `j` ever become equal, it means they point to the same candidate. To continue the comparison, we need two distinct candidates, so we simply increment `j` to `j+1`.

5.  **Final Result**: After the loop terminates, one of `i` or `j` (specifically, `std::min(i, j)`) will point to the starting index of the lexicographically smallest rotation within the original string `s`. We then extract the substring of length `N` starting from this index in `s_double`.

This algorithm efficiently prunes the search space by eliminating entire blocks of rotations that are guaranteed not to be the smallest, leading to its linear time complexity.

## Complexity Analysis
-   **Time Complexity**: $O(N)$
    *   Creating `s_double` takes $O(N)$ time.
    *   The `while` loop iterates at most $2N$ times. In each iteration, either `k` is incremented, or `i` or `j` (or both, indirectly) are advanced. The total advancement of `i` and `j` combined is at most $2N$. The total increments of `k` are also bounded by $2N$. Thus, the loop runs in $O(N)$ time.
    *   Extracting the final substring using `substr` takes $O(N)$ time.
    *   Therefore, the overall time complexity is $O(N)$.

-   **Space Complexity**: $O(N)$
    *   The `s_double` string requires $O(N)$ space to store $2N$ characters.
    *   Other variables (`i`, `j`, `k`, `n`) use $O(1)$ auxiliary space.
    *   Therefore, the overall space complexity is $O(N)$.

## Solution Code
```cpp
#include <string>
#include <algorithm> // Required for std::min
#include <vector> // Not strictly needed for this solution, but good practice for competitive programming

class Solution {
public:
    // Function to find the lexicographically smallest string after rotating the string left any number of times.
    // Renamed from findLexSmallestRotation to lexiString to match the driver code.
    std::string lexiString(std::string s) {
        int n = s.length();
        
        // Handle empty string case, though constraints state 1 <= s.size().
        if (n == 0) {
            return "";
        }

        // Concatenate s with itself. Any rotation of s is a substring of s_double of length n.
        // For example, if s = "abcd", s_double = "abcdabcd".
        // Rotations: "abcd" (s_double[0...3]), "bcda" (s_double[1...4]), etc.
        std::string s_double = s + s;

        // i and j are pointers to the starting indices of two candidate smallest rotations.
        // k is the length of the common prefix between the rotations starting at i and j.
        int i = 0; 
        int j = 1; 
        int k = 0; 

        // The loop continues as long as both i and j are valid starting positions
        // within the first N characters of s_double (i.e., 0 to N-1),
        // and k (common prefix length) does not exceed N (meaning we haven't compared full rotations yet).
        while (i < n && j < n && k < n) {
            // Compare characters at current positions (i+k) and (j+k) in s_double.
            if (s_double[i + k] == s_double[j + k]) {
                k++; // Common prefix extends, continue comparing next characters.
            } else {
                // If characters differ, one candidate is lexicographically smaller.
                // The larger one cannot be the smallest rotation.
                if (s_double[i + k] < s_double[j + k]) {
                    // The rotation starting at i is smaller.
                    // Any rotation starting from j up to j+k would have s_double[j...j+k]
                    // as a prefix, which is identical to s_double[i...i+k].
                    // But s_double[j+k] is greater than s_double[i+k], so s_double[j...]
                    // is definitely larger than s_double[i...].
                    // We can safely advance j past all positions that start with a prefix
                    // that is known to be larger than s_double[i...].
                    // The next candidate for j should be j + k + 1.
                    j = j + k + 1;
                } else { // s_double[i + k] > s_double[j + k]
                    // The rotation starting at j is smaller.
                    // Similar logic, advance i past all positions that start with a prefix
                    // that is known to be larger than s_double[j...].
                    // The next candidate for i should be i + k + 1.
                    i = i + k + 1;
                }
                k = 0; // Reset common prefix length as we've advanced a pointer.
            }

            // Ensure i and j are distinct. If they become equal, advance j.
            // This is crucial because we are comparing two distinct candidate rotations.
            // If i and j point to the same start, they represent the same rotation,
            // and we need a second distinct candidate.
            if (i == j) {
                j++;
            }
        }

        // After the loop, one of i or j (whichever is smaller and still within [0, N-1])
        // will point to the start of the lexicographically smallest rotation.
        // The algorithm guarantees that the minimum of the final i and j (before they potentially
        // went out of bounds) will be the correct starting index.
        int start_index = std::min(i, j);

        // Return the substring of length N starting at start_index from s_double.
        // This substring is the lexicographically smallest rotation.
        return s_double.substr(start_index, n);
    }
};
```