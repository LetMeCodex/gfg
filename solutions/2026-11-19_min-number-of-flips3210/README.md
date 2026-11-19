# [Min Flips for Alternate](https://www.geeksforg eeks.org/problems/min-number-of-flips3210/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 1 attempt(s)

## Problem Summary

Given a binary string `S`, the task is to find the minimum number of flips required to make the string an alternating binary string. An alternating binary string is one where adjacent characters are different (e.g., "010101..." or "101010..."). A flip involves changing a '0' to a '1' or a '1' to a '0'.

For example:
- Input: `S = "001"`
- Output: `1` (Change "001" to "010" by flipping `S[1]`, or to "101" by flipping `S[0]` and `S[2]`. Minimum is 1 flip.)

## Intuition & Approach

The core idea behind solving this problem is to recognize that there are only two possible patterns for an alternating binary string of any given length:
1.  **Starts with '0'**: `010101...` (e.g., "010", "0101", "01010")
2.  **Starts with '1'**: `101010...` (e.g., "101", "1010", "10101")

Our goal is to transform the given string `S` into *one of these two patterns* using the minimum number of flips. We can achieve this by calculating the flips needed for each pattern independently and then taking the minimum of the two counts.

**Detailed Approach:**

1.  **Initialize Counters**:
    *   `flips1`: To store the number of flips required to make `S` match the pattern starting with '0' (`010101...`).
    *   `flips2`: To store the number of flips required to make `S` match the pattern starting with '1' (`101010...`).

2.  **Iterate Through the String**:
    *   Loop through the input string `S` from index `i = 0` to `n-1` (where `n` is the length of `S`).

3.  **Calculate `flips1` (for target `010101...`)**:
    *   For this pattern, characters at even indices (`0, 2, 4, ...`) should be '0', and characters at odd indices (`1, 3, 5, ...`) should be '1'.
    *   If `i` is an even index:
        *   If `S[i]` is '1', it needs to be flipped to '0'. Increment `flips1`.
    *   If `i` is an odd index:
        *   If `S[i]` is '0', it needs to be flipped to '1'. Increment `flips1`.

4.  **Calculate `flips2` (for target `101010...`)**:
    *   For this pattern, characters at even indices (`0, 2, 4, ...`) should be '1', and characters at odd indices (`1, 3, 5, ...`) should be '0'.
    *   If `i` is an even index:
        *   If `S[i]` is '0', it needs to be flipped to '1'. Increment `flips2`.
    *   If `i` is an odd index:
        *   If `S[i]` is '1', it needs to be flipped to '0'. Increment `flips2`.

5.  **Return Minimum**:
    *   After iterating through the entire string, `flips1` will hold the total flips for the first pattern, and `flips2` for the second.
    *   The minimum number of flips required is `std::min(flips1, flips2)`.

This approach is efficient because it processes each character of the string exactly once to determine its contribution to both potential target patterns.

## Complexity Analysis

-   **Time Complexity**: $O(N)$
    *   The solution involves a single loop that iterates through the input string `S` once. Inside the loop, constant-time operations (character comparisons, arithmetic operations, counter increments) are performed. Therefore, the time taken grows linearly with the length of the string `N`.

-   **Space Complexity**: $O(1)$
    *   The solution uses a few integer variables (`n`, `flips1`, `flips2`) to store counts. These variables consume a constant amount of memory regardless of the input string's length. No additional data structures are allocated based on the input size.

## Solution Code

```cpp
#include <string>
#include <algorithm> // Required for std::min
#include <vector>    // Not strictly needed for this solution, but often included

class Solution {
public:
    int minFlips(std::string s) {
        int n = s.length();
        int flips1 = 0; // Flips to make it start with '0' (target: 010101...)
        int flips2 = 0; // Flips to make it start with '1' (target: 101010...)

        for (int i = 0; i < n; ++i) {
            // --- Calculate flips for target pattern: 010101... ---
            // At even indices (0, 2, 4...), target is '0'
            // At odd indices (1, 3, 5...), target is '1'
            if (i % 2 == 0) { // Even index
                if (s[i] == '1') { // Current char is '1', but should be '0'
                    flips1++;
                }
            } else { // Odd index
                if (s[i] == '0') { // Current char is '0', but should be '1'
                    flips1++;
                }
            }

            // --- Calculate flips for target pattern: 101010... ---
            // At even indices (0, 2, 4...), target is '1'
            // At odd indices (1, 3, 5...), target is '0'
            if (i % 2 == 0) { // Even index
                if (s[i] == '0') { // Current char is '0', but should be '1'
                    flips2++;
                }
            } else { // Odd index
                if (s[i] == '1') { // Current char is '1', but should be '0'
                    flips2++;
                }
            }
        }

        // The minimum flips will be the smaller of the two possibilities
        return std::min(flips1, flips2);
    }
};

```