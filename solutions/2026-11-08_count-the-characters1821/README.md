# [Count Group Occurrences](https://www.geeksforgeeks.org/problems/count-the-characters1821/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to count the number of distinct characters that appear exactly `k` times as a "group" in a given string `s`. A group is defined as a sequence of one or more identical characters. For example, in the string "aaabbc", 'a' forms one group ("aaa"), 'b' forms one group ("bb"), and 'c' forms one group ("c"). If the string were "aaabbcaac", 'a' would form two groups ("aaa" and "aa"), 'b' one group ("bb"), and 'c' one group ("c"). We need to find how many *distinct* characters have a group count exactly equal to `k`.

**Example:**
`s = "aaabbcaac"`, `k = 2`
- 'a': forms two groups ("aaa", "aa") -> count = 2
- 'b': forms one group ("bb") -> count = 1
- 'c': forms one group ("c") -> count = 1

Only 'a' has a group count of 2 (which is `k`). So, the answer is 1.

## Intuition & Approach

### Intuition

The core idea is to traverse the string and identify these "groups" of consecutive identical characters. For each character, we need to maintain a count of how many distinct groups it forms. Once we have processed the entire string and tallied the group counts for all characters, we can then easily check which characters have a group count exactly equal to `k`.

### Approach

1.  **Initialize Group Counts:** We'll use an array (or `std::vector`) of size 26 to store the group counts for each lowercase English alphabet character. `group_counts[0]` will store the count for 'a', `group_counts[1]` for 'b', and so on. Initialize all counts to zero.

2.  **Iterate and Identify Groups:**
    *   Use a pointer `i` to traverse the input string `s` from left to right.
    *   When `i` points to a character `s[i]`, this character signifies the beginning of a new group.
    *   Increment the group count for `s[i]` in our `group_counts` array.
    *   Now, we need to skip all subsequent characters that are part of *this same group*. We do this by advancing `i` as long as `i+1` is within string bounds and `s[i+1]` is the same as `s[i]`. This inner loop effectively moves `i` to the last character of the current group.
    *   After the inner loop finishes, `i` is at the end of the current group. To ensure we start processing the *next* potential group (which might be a different character or a new group of the same character if there was a break), we must increment `i` one more time. This moves `i` past the current group.

3.  **Final Count:** After the main `while` loop finishes (meaning we have processed the entire string), iterate through our `group_counts` array (from index 0 to 25). For each character, if its stored group count is exactly equal to `k`, increment a final result counter.

4.  **Return Result:** Return the final result counter.

This approach ensures that each group is identified exactly once and its corresponding character's group count is incremented correctly.

## Complexity Analysis

*   **Time Complexity**: $O(N)$
    *   The main `while (i < n)` loop iterates through the string.
    *   The inner `while (i + 1 < n && s[i+1] == current_char)` loop also advances the pointer `i`.
    *   Crucially, the pointer `i` is always incremented in both the outer and inner loops and never moves backward. Each character in the string `s` is visited and processed at most a constant number of times (once as `s[i]` at the start of a group, and potentially multiple times as `s[i+1]` within the same group). Therefore, the total traversal of the string takes linear time, $O(N)$, where $N$ is the length of the string `s`.
    *   After traversing the string, we iterate through the `group_counts` array, which has a fixed size of 26. This takes $O(26)$, which is constant time, $O(1)$.
    *   Combining these, the total time complexity is $O(N) + O(1) = O(N)$.

*   **Space Complexity**: $O(1)$
    *   We use a `std::vector<int> group_counts` of size 26 to store the group counts for each lowercase English alphabet character. This size is constant and does not depend on the length of the input string `N`.
    *   Therefore, the space complexity is $O(26)$, which simplifies to $O(1)$.

## Solution Code

```cpp
#include <string> // Required for std::string
#include <vector> // Required for std::vector

// The problem asks to implement the solution inside a class Solution.
class Solution {
public:
    // Function signature deduced from problem statement and input format.
    // Problem title: "Count Group Occurrences" suggests the method name.
    // Renamed from 'countGroupOccurrences' to 'getCount' to match the driver's expectation.
    int getCount(std::string s, int k) { // Changed function name here
        // An array (or std::vector) to store the count of groups for each character.
        // Since s consists only of lowercase English alphabets, an array of size 26 is sufficient.
        // group_counts[0] will store the group count for 'a', group_counts[1] for 'b', and so on.
        // Initialize all counts to 0.
        std::vector<int> group_counts(26, 0); 

        int n = s.length(); // Get the length of the input string.
        int i = 0; // Initialize a pointer to iterate through the string.

        // Iterate through the string to identify groups and count them.
        while (i < n) {
            char current_char = s[i]; // Get the character at the current position.
            
            // This marks the beginning of a new group for 'current_char'.
            // Increment the group count for this character.
            group_counts[current_char - 'a']++;

            // Skip all consecutive occurrences of the current character.
            // This inner loop effectively moves 'i' to the last character of the current group.
            while (i + 1 < n && s[i+1] == current_char) {
                i++;
            }
            
            // After the inner loop, 'i' is at the end of the current group.
            // To start processing the next potential group (which might be a different character
            // or a new group of the same character if there was a break),
            // we increment 'i' to move past the current group.
            i++; 
        }

        // After processing the entire string, count how many distinct characters
        // have a group count exactly equal to 'k'.
        int distinct_chars_with_k_groups = 0;
        for (int j = 0; j < 26; ++j) { // Iterate through all possible lowercase English alphabets.
            if (group_counts[j] == k) {
                distinct_chars_with_k_groups++; // If the count matches 'k', increment our result.
            }
        }

        return distinct_chars_with_k_groups; // Return the final count.
    }
};
```