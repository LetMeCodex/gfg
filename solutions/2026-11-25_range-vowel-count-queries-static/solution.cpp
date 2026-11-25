#include <vector>
#include <string>

class Solution {
public:
    // Helper function to check if a character is a vowel
    // This function is private as it's an internal utility for the class.
    bool isVowel(char c) {
        // Convert to lowercase to handle both uppercase and lowercase vowels if needed,
        // though the problem description and examples suggest lowercase input.
        // For strict adherence to 'a', 'e', 'i', 'o', 'u', no conversion is needed.
        // Assuming input string 's' contains only lowercase English letters based on typical GFG constraints.
        return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    }

    // Function to solve the Range Vowel Count Queries problem
    // Renamed from 'solve' to 'countVowels' to match the GFG driver code's expectation.
    std::vector<int> countVowels(std::string s, std::vector<std::vector<int>> queries) {
        int n = s.length();
        
        // Create a prefix sum array.
        // prefix_vowels[i] will store the number of vowels in s[0...i-1].
        // Therefore, it needs to be of size n+1.
        std::vector<int> prefix_vowels(n + 1, 0);
        
        // Build the prefix sum array
        // Iterate from i=0 to n-1 (for characters s[0] to s[n-1])
        // The corresponding prefix_vowels index will be i+1.
        for (int i = 0; i < n; ++i) {
            prefix_vowels[i+1] = prefix_vowels[i]; // Copy count from previous prefix
            if (isVowel(s[i])) {
                prefix_vowels[i+1]++; // Increment if current character is a vowel
            }
        }
        
        // Process each query
        std::vector<int> results;
        // Reserve memory to avoid reallocations, improving performance slightly for large number of queries.
        results.reserve(queries.size()); 
        
        for (const auto& query : queries) {
            int l = query[0]; // Left boundary of the query range (0-indexed)
            int r = query[1]; // Right boundary of the query range (0-indexed)
            
            // Calculate the number of vowels in s[l...r] using prefix sums.
            // prefix_vowels[r+1] contains the count of vowels in s[0...r].
            // prefix_vowels[l] contains the count of vowels in s[0...l-1].
            // Their difference gives the count in s[l...r].
            results.push_back(prefix_vowels[r+1] - prefix_vowels[l]);
        }
        
        return results;
    }
};