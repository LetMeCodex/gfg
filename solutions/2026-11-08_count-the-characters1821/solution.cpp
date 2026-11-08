#include <string> // Required for std::string
#include <vector> // Required for std::vector

// The problem asks to implement the solution inside a class Solution.
class Solution {
public:
    // Function signature deduced from problem statement and input format.
    // Problem title: "Count Group Occurrences" suggests the method name.
    // Renamed from 'countGroupOccurrences' to 'getCount' to match the driver's expectation.
    int getCount(std::string s, int k) { // Changed function name here
        // An array (or std::vector) to store the count of groups for each character.
        // Since s consists only of lowercase English alphabets, an array of size 26 is sufficient.
        // group_counts[0] will store the group count for 'a', group_counts[1] for 'b', and so on.
        // Initialize all counts to 0.
        std::vector<int> group_counts(26, 0); 

        int n = s.length(); // Get the length of the input string.
        int i = 0; // Initialize a pointer to iterate through the string.

        // Iterate through the string to identify groups and count them.
        while (i < n) {
            char current_char = s[i]; // Get the character at the current position.
            
            // This marks the beginning of a new group for 'current_char'.
            // Increment the group count for this character.
            group_counts[current_char - 'a']++;

            // Skip all consecutive occurrences of the current character.
            // This inner loop effectively moves 'i' to the last character of the current group.
            while (i + 1 < n && s[i+1] == current_char) {
                i++;
            }
            
            // After the inner loop, 'i' is at the end of the current group.
            // To start processing the next potential group (which might be a different character
            // or a new group of the same character if there was a break),
            // we increment 'i' to move past the current group.
            i++; 
        }

        // After processing the entire string, count how many distinct characters
        // have a group count exactly equal to 'k'.
        int distinct_chars_with_k_groups = 0;
        for (int j = 0; j < 26; ++j) { // Iterate through all possible lowercase English alphabets.
            if (group_counts[j] == k) {
                distinct_chars_with_k_groups++; // If the count matches 'k', increment our result.
            }
        }

        return distinct_chars_with_k_groups; // Return the final count.
    }
};