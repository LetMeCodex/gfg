#include <string>
#include <vector>

class Solution {
public:
    bool isItPossible(std::string s1, std::string s2) { // Renamed from canTransform to isItPossible
        if (s1.length() != s2.length()) {
            return false;
        }

        int n = s1.length();
        int i = 0; // Pointer for s1
        int j = 0; // Pointer for s2

        while (i < n || j < n) {
            // Skip '#' characters in s1
            while (i < n && s1[i] == '#') {
                i++;
            }
            // Skip '#' characters in s2
            while (j < n && s2[j] == '#') {
                j++;
            }

            // If one string has remaining non-'#' characters while the other doesn't
            if ((i < n && j == n) || (i == n && j < n)) {
                return false;
            }
            // If both strings have reached their ends simultaneously
            if (i == n && j == n) {
                break;
            }

            // At this point, s1[i] and s2[j] are non-'#' characters.
            // They must be the same character to maintain relative order.
            if (s1[i] != s2[j]) {
                return false;
            }

            // Apply movement constraints:
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

            // Move to the next non-'#' character in both strings
            i++;
            j++;
        }

        return true;
    }
};