Here's a high-quality GitHub solution writeup for the Range Vowel Count Queries problem:

---

# [Range Vowel Count Queries](https://www.geeksforgeeks.org/problems/range-vowel-count-queries-static/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to process a series of queries on a given string `s`. Each query consists of two 0-indexed integers, `l` and `r`, representing a range `[l, r]` within the string. For each query, we need to count the total number of vowels ('a', 'e', 'i', 'o', 'u') present in the substring `s[l...r]` (inclusive). The final output should be a list of these counts, one for each query.

**Example:**
If `s = "geeksforgeeks"` and a query is `[1, 4]`, the substring is `s[1...4] = "eeks"`. The vowels are 'e', 'e', so the count is 2.

## Intuition & Approach

### Initial Thought (Naive Approach)

A straightforward approach would be to iterate through the substring `s[l...r]` for each query and count the vowels.
*   For each query `(l, r)`:
    *   Initialize `count = 0`.
    *   Iterate `i` from `l` to `r`.
    *   If `s[i]` is a vowel, increment `count`.
    *   Add `count` to results.

**Why this is inefficient:** If the string length `N` and the number of queries `Q` are large (e.g., up to `10^5`), iterating through the substring for each query would take `O(N)` time per query in the worst case. This leads to a total time complexity of `O(N * Q)`, which would be `10^5 * 10^5 = 10^{10}` operations in the worst case, resulting in a Time Limit Exceeded (TLE).

### Optimized Approach: Prefix Sums

Since we need to answer multiple range sum queries (counting vowels is a form of summing 1 for a vowel and 0 for a consonant), the **Prefix Sum** technique is ideal.

The idea is to pre-process the string once to build an auxiliary array that allows us to calculate the sum (vowel count) for any range `[l, r]` in `O(1)` time.

**Steps:**

1.  **Create a `prefix_vowels` array:**
    *   Initialize a `std::vector<int> prefix_vowels` of size `N + 1` (where `N` is the length of string `s`) with all zeros.
    *   `prefix_vowels[i]` will store the total count of vowels in the substring `s[0...i-1]`.
    *   `prefix_vowels[0]` will be 0 (no vowels in an empty prefix).

2.  **Build the `prefix_vowels` array:**
    *   Iterate through the string `s` from `i = 0` to `N-1`.
    *   For each character `s[i]`:
        *   Copy the previous prefix sum: `prefix_vowels[i+1] = prefix_vowels[i]`.
        *   Check if `s[i]` is a vowel using a helper function `isVowel()`.
        *   If `s[i]` is a vowel, increment `prefix_vowels[i+1]` by 1.
    *   After this loop, `prefix_vowels[k]` will correctly store the count of vowels in `s[0...k-1]`.

3.  **Process Queries:**
    *   Initialize an empty `std::vector<int> results` to store the answers for each query.
    *   For each query `(l, r)`:
        *   The number of vowels in `s[l...r]` can be calculated as:
            `count = prefix_vowels[r+1] - prefix_vowels[l]`
        *   **Explanation:**
            *   `prefix_vowels[r+1]` contains the count of vowels from `s[0]` up to `s[r]`.
            *   `prefix_vowels[l]` contains the count of vowels from `s[0]` up to `s[l-1]`.
            *   Subtracting `prefix_vowels[l]` from `prefix_vowels[r+1]` effectively removes the count of vowels before index `l`, leaving only the count for the desired range `s[l...r]`.
        *   Add this `count` to the `results` vector.

4.  **Return `results`:** The vector containing all query answers.

This approach pre-processes the string once in `O(N)` time and then answers each query in `O(1)` time, making the overall solution much more efficient.

## Complexity Analysis

*   **Time Complexity**:
    *   **Building the `prefix_vowels` array**: We iterate through the string `s` once, which takes `O(N)` time, where `N` is the length of the string. The `isVowel` check is `O(1)`.
    *   **Processing queries**: We iterate through `Q` queries. Each query is answered in `O(1)` time using the precomputed prefix sum array.
    *   **Total Time Complexity**: $O(N + Q)$

*   **Space Complexity**:
    *   **`prefix_vowels` array**: We create an auxiliary array of size `N+1` to store prefix sums. This requires `O(N)` space.
    *   **`results` array**: We store `Q` results for the queries. This requires `O(Q)` space.
    *   **Total Space Complexity**: $O(N + Q)$

## Solution Code

```cpp
#include <vector>
#include <string>

class Solution {
public:
    // Helper function to check if a character is a vowel
    // This function is private as it's an internal utility for the class.
    bool isVowel(char c) {
        // Convert to lowercase to handle both uppercase and lowercase vowels if needed,
        // though the problem description and examples suggest lowercase input.
        // For strict adherence to 'a', 'e', 'i', 'o', 'u', no conversion is needed.
        // Assuming input string 's' contains only lowercase English letters based on typical GFG constraints.
        return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    }

    // Function to solve the Range Vowel Count Queries problem
    // Renamed from 'solve' to 'countVowels' to match the GFG driver code's expectation.
    std::vector<int> countVowels(std::string s, std::vector<std::vector<int>> queries) {
        int n = s.length();
        
        // Create a prefix sum array.
        // prefix_vowels[i] will store the number of vowels in s[0...i-1].
        // Therefore, it needs to be of size n+1.
        std::vector<int> prefix_vowels(n + 1, 0);
        
        // Build the prefix sum array
        // Iterate from i=0 to n-1 (for characters s[0] to s[n-1])
        // The corresponding prefix_vowels index will be i+1.
        for (int i = 0; i < n; ++i) {
            prefix_vowels[i+1] = prefix_vowels[i]; // Copy count from previous prefix
            if (isVowel(s[i])) {
                prefix_vowels[i+1]++; // Increment if current character is a vowel
            }
        }
        
        // Process each query
        std::vector<int> results;
        // Reserve memory to avoid reallocations, improving performance slightly for large number of queries.
        results.reserve(queries.size()); 
        
        for (const auto& query : queries) {
            int l = query[0]; // Left boundary of the query range (0-indexed)
            int r = query[1]; // Right boundary of the query range (0-indexed)
            
            // Calculate the number of vowels in s[l...r] using prefix sums.
            // prefix_vowels[r+1] contains the count of vowels in s[0...r].
            // prefix_vowels[l] contains the count of vowels in s[0...l-1].
            // Their difference gives the count in s[l...r].
            results.push_back(prefix_vowels[r+1] - prefix_vowels[l]);
        }
        
        return results;
    }
};
```