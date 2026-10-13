#include <string>
#include <vector>
#include <numeric>

class Solution {
public:
    // Renamed from rollCharacters to findRollOut to match the driver code
    std::string findRollOut(std::string s, std::vector<int>& roll) {
        int n = s.length();
        // diff array stores the net change in roll operations at each index.
        // diff[i] means how many roll operations start at index i.
        // diff[i] - diff[i+1] gives the total number of roll operations
        // that affect index i.
        std::vector<int> diff(n + 1, 0);

        for (int r : roll) {
            // Each roll operation of length 'r' increments the count of
            // active roll operations starting from index 0.
            diff[0]++;
            // If the roll operation extends beyond the string length,
            // it doesn't affect any character beyond index n-1.
            // So, we decrement the count of active roll operations at index 'r'.
            // This is a standard technique for range updates using difference arrays.
            if (r < n) {
                diff[r]--;
            }
        }

        int current_roll_count = 0;
        for (int i = 0; i < n; ++i) {
            // current_roll_count accumulates the net effect of all roll operations
            // that start at or before index 'i' and end after index 'i'.
            current_roll_count += diff[i];

            // Get the original character's value (0 for 'a', 1 for 'b', etc.)
            int original_char_val = s[i] - 'a';

            // Apply the total roll count to the character.
            // The modulo 26 handles the wrap-around from 'z' to 'a'.
            int new_char_val = (original_char_val + current_roll_count) % 26;

            // Convert the new character value back to a character and update the string.
            s[i] = 'a' + new_char_val;
        }

        return s;
    }
};