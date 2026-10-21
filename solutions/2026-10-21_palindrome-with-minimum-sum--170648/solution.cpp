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