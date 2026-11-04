#include <string>
#include <vector>
#include <numeric>

class Solution {
public:
    // Helper function to check if the condition is met after 'k' days.
    // 'n' is the length of the string.
    // 'original_s' is the initial string.
    // 'p' is the permutation array.
    bool check(int k, int n, const std::string& original_s, const std::vector<int>& p) {
        // Create a mutable copy of the string to apply replacements.
        std::string current_s = original_s; 
        
        // Apply 'k' replacements based on the permutation array 'p'.
        for (int i = 0; i < k; ++i) {
            current_s[p[i]] = '?';
        }
        
        // Check the condition: no two adjacent non-'?' characters are equal.
        for (int i = 0; i < n - 1; ++i) {
            // If both characters are not '?' AND they are equal, then the condition is violated.
            if (current_s[i] != '?' && current_s[i+1] != '?' && current_s[i] == current_s[i+1]) {
                return false; // Condition violated, 'k' days are not enough.
            }
        }
        
        return true; // Condition met, 'k' days are sufficient.
    }

    // Main function to find the minimum number of days.
    // Renamed from minimumDays to getMinimumDays to match the driver code.
    int getMinimumDays(std::string s, std::vector<int> p) {
        int n = s.length();
        
        // Binary search for the minimum 'k' (number of days).
        int low = 0;
        int high = n; 
        int ans = n; 
        
        while (low <= high) {
            int mid = low + (high - low) / 2; 
            
            if (check(mid, n, s, p)) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        return ans; 
    }
};