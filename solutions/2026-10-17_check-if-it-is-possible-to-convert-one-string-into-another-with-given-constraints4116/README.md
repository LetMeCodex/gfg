# [Convert One String to Another with Constraints](https://www.geeksforgeeks.org/problems/check-if-it-is-possible-to-convert-one-string-into-another-with-given-constraints4116/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 3 attempt(s)

## Problem Summary

The problem asks us to determine if a given string `s1` can be transformed into another string `s2` by applying specific movement rules to its characters. Both strings consist of 'A', 'B', and '#'. The rules are:
1.  'A' characters can only move to the left (i.e., swap with an adjacent '#' character to its left).
2.  'B' characters can only move to the right (i.e., swap with an adjacent '#' character to its right).
3.  '#' characters are static and cannot move, but they act as empty spaces that 'A's and 'B's can move into.
4.  The relative order of 'A's and 'B's must be preserved. For example, if `s1` has `A` at index `i` and `B` at index `k` where `i < k`, then in `s2`, the corresponding `A` must appear before the corresponding `B`.

We need to return `true` if `s1` can be transformed into `s2` under these constraints, and `false` otherwise.

## Intuition & Approach

The core idea revolves around two main observations:

1.  **Relative Order Preservation**: The problem statement explicitly mentions that the relative order of 'A's and 'B's must be preserved. This implies that if we remove all '#' characters from both `s1` and `s2`, the resulting non-'#' strings must be identical. For example, if `s1 = "A#B"` and `s2 = "#AB"`, after removing '#', both become "AB". This is a necessary condition.

2.  **Movement Constraints**:
    *   An 'A' can only move left. This means if an 'A' is at index `i` in `s1` and its corresponding 'A' is at index `j` in `s2`, then `j` must be less than or equal to `i` (`j <= i`). If `j > i`, it means 'A' moved right, which is forbidden.
    *   A 'B' can only move right. Similarly, if a 'B' is at index `i` in `s1` and its corresponding 'B' is at index `j` in `s2`, then `j` must be greater than or equal to `i` (`j >= i`). If `j < i`, it means 'B' moved left, which is forbidden.

These observations lead to a **two-pointer approach**:

1.  **Initial Check**: First, ensure that `s1` and `s2` have the same length. If not, transformation is impossible.
2.  **Initialize Pointers**: Use two pointers, `i` for `s1` and `j` for `s2`, both starting at index 0.
3.  **Iterate and Skip '#'**:
    *   In a loop, advance pointer `i` past any '#' characters in `s1`.
    *   Similarly, advance pointer `j` past any '#' characters in `s2`.
    *   This effectively makes `i` and `j` point to the *next non-'#' character* in their respective strings.
4.  **Handle End Conditions**:
    *   If one pointer reaches the end of its string (`i == n` or `j == n`) while the other hasn't, it means the count or alignment of non-'#' characters is different, making transformation impossible. Return `false`.
    *   If both pointers reach the end simultaneously (`i == n && j == n`), it means all non-'#' characters have been successfully matched and validated. The transformation is possible. Break the loop and return `true`.
5.  **Character Match**: If both `s1[i]` and `s2[j]` are non-'#' characters, they must be identical to satisfy the relative order preservation. If `s1[i] != s2[j]`, return `false`.
6.  **Apply Movement Constraints**:
    *   If `s1[i]` (and thus `s2[j]`) is 'A': Check if `j > i`. If true, 'A' moved right, which is forbidden. Return `false`.
    *   If `s1[i]` (and thus `s2[j]`) is 'B': Check if `j < i`. If true, 'B' moved left, which is forbidden. Return `false`.
7.  **Advance Pointers**: If all checks pass for the current pair of characters, increment both `i` and `j` to move to the next non-'#' characters.
8.  **Final Result**: If the loop completes without returning `false`, it means all conditions were met, and `s1` can be transformed into `s2`. Return `true`.

This two-pointer strategy efficiently compares the "significant" characters ('A' and 'B') while implicitly handling the '#' characters as flexible spaces. The `i` and `j` pointers effectively track the *original* and *final* positions of the *k-th* non-'#' character, allowing for direct application of the movement rules.

## Complexity Analysis

*   **Time Complexity**: $O(N)$, where $N$ is the length of the strings `s1` and `s2`.
    *   The main `while` loop iterates at most `N` times.
    *   Inside the loop, the inner `while` loops (skipping '#' characters) also advance `i` and `j` at most `N` times in total across all iterations. Each character in both strings is visited by its respective pointer at most once.
    *   All operations within the loop (comparisons, increments) are constant time.
    *   Therefore, the total time complexity is linear with respect to the string length.

*   **Space Complexity**: $O(1)$.
    *   The solution uses a few integer variables (`n`, `i`, `j`) for pointers and loop control.
    *   No auxiliary data structures are used that scale with the input size.
    *   Therefore, the space complexity is constant.

## Solution Code

```cpp
#include <string>
// #include <vector> // Not strictly necessary for this solution

class Solution {
public:
    bool isItPossible(std::string s1, std::string s2) {
        // Step 1: Initial length check
        if (s1.length() != s2.length()) {
            return false;
        }

        int n = s1.length();
        int i = 0; // Pointer for s1
        int j = 0; // Pointer for s2

        // Loop as long as either pointer has not reached the end of its string
        while (i < n || j < n) {
            // Step 3: Advance pointer i past '#' characters in s1
            while (i < n && s1[i] == '#') {
                i++;
            }
            // Step 3: Advance pointer j past '#' characters in s2
            while (j < n && s2[j] == '#') {
                j++;
            }

            // Step 4: Handle end conditions
            // If one string has remaining non-'#' characters while the other doesn't
            if ((i < n && j == n) || (i == n && j < n)) {
                return false;
            }
            // If both strings have reached their ends simultaneously, all checks passed
            if (i == n && j == n) {
                break; // Exit the loop, transformation is possible
            }

            // At this point, s1[i] and s2[j] are non-'#' characters.
            // Step 5: They must be the same character to maintain relative order.
            if (s1[i] != s2[j]) {
                return false;
            }

            // Step 6: Apply movement constraints
            // 'A' can only move left, so its final position (j) must be <= its initial position (i).
            if (s1[i] == 'A') {
                if (j > i) { // 'A' moved right, which is not allowed
                    return false;
                }
            }
            // 'B' can only move right, so its final position (j) must be >= its initial position (i).
            else { // s1[i] == 'B'
                if (j < i) { // 'B' moved left, which is not allowed
                    return false;
                }
            }

            // Step 7: Move to the next non-'#' character in both strings
            i++;
            j++;
        }

        // Step 8: If the loop completes, all conditions were met
        return true;
    }
};
```