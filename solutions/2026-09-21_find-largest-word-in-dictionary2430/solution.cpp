#include <vector>
#include <string>
#include <algorithm>

class Solution {
public:
    std::string findLongestWord(std::string s, std::vector<std::string>& d) {
        // Sort dictionary: 
        // 1. Longer words first
        // 2. If lengths equal, lexicographically smaller first
        std::sort(d.begin(), d.end(), [](const std::string& a, const std::string& b) {
            if (a.length() != b.length()) {
                return a.length() > b.length();
            }
            return a < b;
        });

        for (const std::string& word : d) {
            int i = 0; // pointer for s
            int j = 0; // pointer for word
            int s_len = s.length();
            int w_len = word.length();

            // Two-pointer check
            while (i < s_len && j < w_len) {
                if (s[i] == word[j]) {
                    j++;
                }
                i++;
            }

            // If we matched the entire word, since we sorted, 
            // this is guaranteed to be the best answer.
            if (j == w_len) {
                return word;
            }
        }

        return "";
    }
};