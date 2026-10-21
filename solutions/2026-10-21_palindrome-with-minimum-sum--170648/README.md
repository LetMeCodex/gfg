# Palindrome with Replacement

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary

Given a string `s` consisting of lowercase English letters and '?' characters, we need to replace each '?' with a lowercase English letter such that the resulting string is a palindrome. The goal is to minimize the sum of absolute differences between adjacent characters in the resulting palindrome string. If it's impossible to form a palindrome, return -1.

## Intuition & Approach

The problem has two main constraints:
1.  The resulting string must be a palindrome.
2.  The sum of absolute differences between adjacent characters must be minimized.

We can break down the solution into two passes:

**Pass 1: Enforcing the Palindrome Property and Handling Conflicts**

The first and most critical step is to ensure the string can *become* a palindrome. We iterate from the beginning of the string up to its middle (`n/2`). For each index `i`, we consider the character `s[i]` and its symmetric counterpart `s[n - 1 - i]`.

*   **Case 1: Both `s[i]` and `s[n - 1 - i]` are letters.**
    If they are different, it's impossible to form a palindrome, so we immediately return -1. If they are the same, they satisfy the palindrome condition, and we do nothing.

*   **Case 2: One is a letter, and the other is '?'.**
    The '?' must be replaced by the existing letter to satisfy the palindrome property. For example, if `s[i]` is 'a' and `s[n - 1 - i]` is '?', we set `s[n - 1 - i]` to 'a'. Similarly, if `s[i]` is '?' and `s[n - 1 - i]` is 'b', we set `s[i]` to 'b'.

*   **Case 3: Both `s[i]` and `s[n - 1 - i]` are '?'.**
    We leave them as '?' for now. These will be filled in the second pass to minimize the sum of adjacent differences.

After this pass, if we haven't returned -1, we've ensured that the string *can* be made into a palindrome. Any remaining '?' characters are in symmetric pairs (or are the middle character if `n` is odd).

**Pass 2: Filling Remaining '?'s Greedily for Minimum Sum**

Now, we need to fill the remaining '?' characters to minimize the sum of absolute differences between adjacent characters. The key insight here is that to minimize the difference between adjacent characters, we should try to make them as close as possible.

A greedy approach works well:
1.  **Find the first known character:** Scan the string to find the first character that is not '?'. This character will serve as a reference point. If the entire string consists of '?'s, we can pick 'a' as a default.
2.  **Fill '?'s from left to right:**
    *   If `s[0]` is '?', we fill it with the `first_known_char` found in step 1. If no known character was found (string was all '?'), we fill `s[0]` with 'a'.
    *   For any subsequent `s[i]` that is '?', we fill it with the character to its immediate left, `s[i-1]`. This is because `s[i-1]` would have already been determined (either original or filled in this pass), and filling `s[i]` with `s[i-1]` guarantees a difference of 0 between `s[i-1]` and `s[i]`, which is the minimum possible.
3.  **Maintain Palindrome Property:** Crucially, whenever we fill `s[i]`, we must also update its symmetric counterpart `s[n - 1 - i]` to be the same character. This ensures the palindrome property is maintained throughout the filling process.

After both passes, the string `s` will be a palindrome with all '?' replaced. The final step is to calculate the sum of absolute differences between all adjacent characters.

**Example Walkthrough:**

Let's consider `s = "a?b?a"`

**Pass 1:**
*   `i = 0`: `s[0] = 'a'`, `s[4] = 'a'`. They match. String remains "a?b?a".
*   `i = 1`: `s[1] = '?'`, `s[3] = '?'`. Both are '?'. Leave them. String remains "a?b?a".
*   `i = 2`: `n/2` is reached.

**Pass 2:**
*   Find first known char: `s[0] = 'a'`. So, `first_known_char = 'a'`.
*   Fill '?'s:
    *   `i = 0`: `s[0]` is 'a' (not '?').
    *   `i = 1`: `s[1]` is '?'. Fill with `s[0]` (which is 'a'). So, `s[1] = 'a'`. Update `s[n-1-1] = s[3]` to 'a'. String becomes "aab?a" -> "aabaa".
    *   `i = 2`: `s[2]` is 'b' (not '?').
    *   `i = 3`: `s[3]` is 'a' (not '?'). (It was already set in the previous step).
    *   `i = 4`: `s[4]` is 'a' (not '?').

The resulting string is "aabaa".

**Calculate Sum:**
*   `abs('a' - 'a') = 0`
*   `abs('a' - 'b') = 1`
*   `abs('b' - 'a') = 1`
*   `abs('a' - 'a') = 0`
Total sum = `0 + 1 + 1 + 0 = 2`.

Let's consider `s = "?ab?"`

**Pass 1:**
*   `i = 0`: `s[0] = '?'`, `s[3] = '?'`. Both '?'. Leave them. String remains "?ab?".
*   `i = 1`: `s[1] = 'a'`, `s[2] = 'b'`. They are different. Impossible to form a palindrome. Return -1.

Let's consider `s = "??a??"`

**Pass 1:**
*   `i = 0`: `s[0] = '?'`, `s[4] = '?'`. Both '?'. Leave them. String remains "??a??".
*   `i = 1`: `s[1] = '?'`, `s[3] = '?'`. Both '?'. Leave them. String remains "??a??".
*   `i = 2`: `n/2` is reached.

**Pass 2:**
*   Find first known char: `s[2] = 'a'`. So, `first_known_char = 'a'`.
*   Fill '?'s:
    *   `i = 0`: `s[0]` is '?'. Fill with `first_known_char` ('a'). So, `s[0] = 'a'`. Update `s[n-1-0] = s[4]` to 'a'. String becomes "aa?a?" -> "aa?aa".
    *   `i = 1`: `s[1]` is '?'. Fill with `s[0]` (which is 'a'). So, `s[1] = 'a'`. Update `s[n-1-1] = s[3]` to 'a'. String becomes "aaa?a" -> "aaaaa".
    *   `i = 2`: `s[2]` is 'a' (not '?').
    *   `i = 3`: `s[3]` is 'a' (not '?'). (Already set).
    *   `i = 4`: `s[4]` is 'a' (not '?'). (Already set).

The resulting string is "aaaaa".

**Calculate Sum:**
*   `abs('a' - 'a') = 0` (four times)
Total sum = `0`.

## Complexity Analysis

*   **Time Complexity**: $O(N)$, where $N$ is the length of the string `s`.
    *   Pass 1 iterates through the first half of the string, taking $O(N/2) = O(N)$ time.
    *   Pass 2 iterates through the entire string twice (once to find the first known character and once to fill '?'s), taking $O(N) + O(N) = O(N)$ time.
    *   The final sum calculation iterates through the string once, taking $O(N)$ time.
    *   Therefore, the total time complexity is $O(N)$.

*   **Space Complexity**: $O(1)$ (or $O(N)$ if string modification is considered as creating a new string, but in-place modification is typical).
    *   We are modifying the input string in-place.
    *   We use a few extra variables (`n`, `left`, `right`, `first_known_char`, `total_diff`), which take constant extra space.
    *   Thus, the space complexity is $O(1)$.

## Solution Code

```cpp
#include <string>
#include <cmath> // For std::abs
#include <algorithm> // Not strictly needed, but good practice for min/max if used.

class Solution {
public:
    long long minimumSum(std::string s) {
        int n = s.length();

        // Pass 1: Enforce palindrome property and handle conflicts.
        // Iterate from the beginning to the middle of the string.
        for (int i = 0; i < n / 2; ++i) {
            char& left = s[i];
            char& right = s[n - 1 - i];

            if (left != '?' && right != '?') {
                // Both characters are letters. They must match.
                if (left != right) {
                    return -1; // Impossible to form a palindrome.
                }
            } else if (left != '?') {
                // Left is a letter, right is '?'. Right must become left.
                right = left;
            } else if (right != '?') {
                // Right is a letter, left is '?'. Left must become right.
                left = right;
            }
            // If both are '?', leave them as '?' for now.
            // They will be filled in Pass 2.
        }

        // Pass 2: Fill remaining '?'s greedily to minimize differences.
        // The strategy is:
        // 1. Find the first non-'?' character in the string.
        // 2. If s[0] is '?', fill it with this first non-'?' char (or 'a' if all are '?').
        // 3. For subsequent '?'s, fill with the character to their left (s[i-1]).
        // 4. Always update the symmetric counterpart s[n-1-i].

        // Step 1: Find the first non-'?' character.
        char first_known_char = '?';
        for (int i = 0; i < n; ++i) {
            if (s[i] != '?') {
                first_known_char = s[i];
                break;
            }
        }

        // Step 2 & 3: Fill '?'s from left to right.
        for (int i = 0; i < n; ++i) {
            if (s[i] == '?') {
                if (i == 0) {
                    // If s[0] is '?', and it's the first char,
                    // it should be filled with first_known_char if available.
                    // If first_known_char is still '?' (meaning the entire string was '?'s),
                    // then 'a' is a safe default.
                    s[i] = (first_known_char != '?') ? first_known_char : 'a';
                } else {
                    // For any other '?', fill it with the character to its left.
                    // s[i-1] is guaranteed to be a non-'?' character at this point,
                    // either original or filled in this pass.
                    s[i] = s[i-1];
                }
                // Update symmetric counterpart.
                // This is crucial for maintaining palindrome property and ensuring
                // that the character chosen for s[i] is reflected.
                // For i > n/2, s[n-1-i] would have been processed earlier and already set.
                // This assignment would be redundant but harmless.
                s[n-1-i] = s[i];
            }
        }

        // Calculate the total sum of absolute ASCII differences between adjacent characters.
        long long total_diff = 0;
        for (int i = 0; i < n - 1; ++i) {
            total_diff += std::abs(s[i] - s[i+1]);
        }

        return total_diff;
    }
};
```