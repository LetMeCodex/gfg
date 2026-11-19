#include <string>
#include <algorithm>
#include <vector>

class Solution {
public:
    int minFlips(std::string s) {
        int n = s.length();
        int flips1 = 0; // Flips to make it start with '0' (010101...)
        int flips2 = 0; // Flips to make it start with '1' (101010...)

        for (int i = 0; i < n; ++i) {
            // Target for starting with '0': 0 at even indices, 1 at odd indices
            if (i % 2 == 0) { // Even index
                if (s[i] == '1') {
                    flips1++;
                }
            } else { // Odd index
                if (s[i] == '0') {
                    flips1++;
                }
            }

            // Target for starting with '1': 1 at even indices, 0 at odd indices
            if (i % 2 == 0) { // Even index
                if (s[i] == '0') {
                    flips2++;
                }
            } else { // Odd index
                if (s[i] == '1') {
                    flips2++;
                }
            }
        }

        return std::min(flips1, flips2);
    }
};