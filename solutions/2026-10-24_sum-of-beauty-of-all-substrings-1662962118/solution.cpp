#include <string>
#include <vector>
#include <algorithm> // Required for std::max and std::min
#include <limits>    // Required for std::numeric_limits

class Solution {
public:
    int beautySum(std::string s) {
        int totalBeauty = 0;
        int n = s.length();

        // Iterate over all possible starting positions of substrings
        for (int i = 0; i < n; ++i) {
            // For each starting position 'i', we will extend the substring to the right.
            // 'freq' will store character frequencies for the current substring s[i...j].
            // It's reset for each new starting position 'i'.
            std::vector<int> freq(26, 0); 

            // Iterate over all possible ending positions 'j' for the current starting position 'i'.
            // This loop effectively generates all substrings starting at 'i'.
            for (int j = i; j < n; ++j) {
                // Add the character s[j] to the frequency count for the substring s[i...j].
                freq[s[j] - 'a']++;

                // Calculate max_freq and min_freq for the current substring.
                int max_freq = 0;
                // Initialize min_freq to a value larger than any possible frequency.
                // std::numeric_limits<int>::max() is a safe choice, as max frequency can be at most N (500).
                int min_freq = std::numeric_limits<int>::max(); 

                // Iterate through the frequency array to find the maximum and minimum frequencies
                // among characters that are actually present in the substring (freq[k] > 0).
                for (int k = 0; k < 26; ++k) {
                    if (freq[k] > 0) { // Only consider characters that have appeared in the substring
                        max_freq = std::max(max_freq, freq[k]);
                        min_freq = std::min(min_freq, freq[k]);
                    }
                }
                
                // The beauty of the current substring is the difference between max and min frequencies.
                // If only one unique character is present, max_freq and min_freq will be equal, resulting in 0 beauty.
                totalBeauty += (max_freq - min_freq);
            }
        }

        return totalBeauty;
    }
};