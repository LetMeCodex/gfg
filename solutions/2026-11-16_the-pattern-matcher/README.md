# [Repeated Pattern Check](https://www.geeksforgeeks.org/problems/the-pattern-matcher/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to determine if a given string `s` can be formed by concatenating one or more occurrences of a pattern of the form "x...xy...y", where the number of 'x' characters is equal to the number of 'y' characters. For example, "xxyy" is valid, "xxxyyy" is valid, but "xxy" is not, and "xyx" is not. The string must consist entirely of such valid "xnyn" blocks concatenated together.

## Intuition & Approach

The core idea is to iterate through the string and identify consecutive blocks of 'x's followed by consecutive blocks of 'y's. For each such block, we need to ensure two conditions are met:

1.  **Starts with 'x'**: A valid "xnyn" block must begin with 'x'. If we encounter a 'y' at the start of a potential block, it's invalid.
2.  **Equal counts of 'x' and 'y'**: The number of 'x's must be exactly equal to the number of 'y's within that block.

We can achieve this by using two pointers or by iterating through the string and keeping track of counts. The provided solution uses a single pointer `i` that marks the beginning of a potential "xnyn" block.

The algorithm proceeds as follows:

1.  Initialize a pointer `i` to the beginning of the string `s`.
2.  While `i` is within the bounds of the string:
    a.  Count the number of consecutive 'x' characters starting from `i`. Let this count be `x_count`.
    b.  If `x_count` is 0, it means the current block doesn't start with 'x', which is invalid. Return `false`.
    c.  Advance a second pointer (implicitly done by updating `j` in the inner loop) to the end of the 'x' sequence.
    d.  Count the number of consecutive 'y' characters starting from the end of the 'x' sequence. Let this count be `y_count`.
    e.  If `y_count` is 0, it means there are no 'y's following the 'x's, which is invalid. Return `false`.
    f.  If `x_count` is not equal to `y_count`, the counts are unequal, which is invalid. Return `false`.
    g.  If both counts are equal and positive, we have found a valid "xnyn" block. Update `i` to the position immediately after this valid block (i.e., `i = j`, where `j` is the pointer after the 'y' sequence).
3.  If the loop completes without returning `false`, it means the entire string `s` has been successfully parsed into valid "xnyn" blocks. Return `true`.

The "2 attempt(s)" likely indicates that the initial attempt might have missed an edge case, such as a string not starting with 'x' or a block with zero 'y's. The provided code correctly handles these by checking `x_count == 0` and implicitly by the structure of the loops (if `j` doesn't advance for 'y's, `y_count` would be 0, and the `x_count != y_count` check would fail if `x_count > 0`).

## Complexity Analysis

-   **Time Complexity**: $O(N)$, where $N$ is the length of the string `s`.
    The outer `while` loop iterates through the string. In each iteration of the outer loop, the inner `while` loops (for counting 'x's and 'y's) collectively advance the pointer `i` to the end of the current "xnyn" block. Since each character in the string is visited at most a constant number of times (once for counting 'x's and once for counting 'y's), the total time complexity is linear with respect to the length of the string.

-   **Space Complexity**: $O(1)$.
    The solution uses a few integer variables (`n`, `i`, `x_count`, `y_count`, `j`) to keep track of indices and counts. The amount of extra space used does not depend on the input string's size. Therefore, the space complexity is constant.

## Solution Code

```cpp
#include <string>
#include <vector>

class Solution {
public:
    /**
     * @brief Checks if a string can be formed by concatenating one or more
     *        occurrences of a pattern "xnyn", where n is a positive integer.
     *
     * @param s The input string to check.
     * @return true if the string matches the pattern, false otherwise.
     */
    bool findPattern(std::string s) {
        int n = s.length();
        int i = 0; // Pointer to the start of the current potential "xnyn" block

        while (i < n) {
            int x_count = 0;
            int j = i; // Temporary pointer to scan for 'x's

            // Count consecutive 'x' characters
            while (j < n && s[j] == 'x') {
                x_count++;
                j++;
            }

            // A valid "xnyn" block must start with at least one 'x'.
            if (x_count == 0) {
                return false;
            }

            int y_count = 0;
            // Count consecutive 'y' characters immediately following 'x's
            while (j < n && s[j] == 'y') {
                y_count++;
                j++;
            }

            // The number of 'x's must equal the number of 'y's in a valid block.
            // Also, there must be at least one 'y' if there were 'x's.
            if (x_count != y_count) {
                return false;
            }

            // If we reached here, a valid "xnyn" block (with x_count == y_count > 0)
            // was found. Move the main pointer 'i' to the end of this block
            // to start searching for the next block.
            i = j;
        }

        // If the loop completes, it means the entire string was successfully
        // parsed into valid "xnyn" blocks.
        return true;
    }
};
```