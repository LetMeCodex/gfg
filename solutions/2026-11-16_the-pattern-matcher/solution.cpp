#include <string>
#include <vector>

class Solution {
public:
    bool findPattern(std::string s) { // Renamed function from repeatedPatternCheck to findPattern
        int n = s.length();
        int i = 0;
        while (i < n) {
            int x_count = 0;
            int j = i;
            while (j < n && s[j] == 'x') {
                x_count++;
                j++;
            }

            if (x_count == 0) {
                return false; // Must start with 'x' for a valid xnyn pattern
            }

            int y_count = 0;
            while (j < n && s[j] == 'y') {
                y_count++;
                j++;
            }

            if (x_count != y_count) {
                return false; // Equal number of 'x' and 'y' required for xnyn
            }
            // If we reached here, a valid xnyn block was found.
            // Move the starting pointer 'i' to the end of this block to find the next one.
            i = j;
        }
        // If the loop completes, it means the entire string was successfully
        // parsed into valid xnyn blocks.
        return true;
    }
};