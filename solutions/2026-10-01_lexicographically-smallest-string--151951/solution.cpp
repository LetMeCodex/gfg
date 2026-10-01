#include <string>
#include <algorithm> // Required for std::min
#include <vector> // Not strictly needed for this solution, but good practice for competitive programming

class Solution {
public:
    // Function to find the lexicographically smallest string after rotating the string left any number of times.
    // Renamed from findLexSmallestRotation to lexiString to match the driver code.
    std::string lexiString(std::string s) {
        int n = s.length();
        
        // Handle empty string case, though constraints state 1 <= s.size().
        if (n == 0) {
            return "";
        }

        // Concatenate s with itself. Any rotation of s is a substring of s_double of length n.
        // For example, if s = "abcd", s_double = "abcdabcd".
        // Rotations: "abcd" (s_double[0...3]), "bcda" (s_double[1...4]), etc.
        std::string s_double = s + s;

        // i and j are pointers to the starting indices of two candidate smallest rotations.
        // k is the length of the common prefix between the rotations starting at i and j.
        int i = 0; 
        int j = 1; 
        int k = 0; 

        // The loop continues as long as both i and j are valid starting positions
        // within the first N characters of s_double (i.e., 0 to N-1),
        // and k (common prefix length) does not exceed N (meaning we haven't compared full rotations yet).
        while (i < n && j < n && k < n) {
            // Compare characters at current positions (i+k) and (j+k) in s_double.
            if (s_double[i + k] == s_double[j + k]) {
                k++; // Common prefix extends, continue comparing next characters.
            } else {
                // If characters differ, one candidate is lexicographically smaller.
                // The larger one cannot be the smallest rotation.
                if (s_double[i + k] < s_double[j + k]) {
                    // The rotation starting at i is smaller.
                    // Any rotation starting from j up to j+k would have s_double[j...j+k]
                    // as a prefix, which is identical to s_double[i...i+k].
                    // But s_double[j+k] is greater than s_double[i+k], so s_double[j...]
                    // is definitely larger than s_double[i...].
                    // We can safely advance j past all positions that start with a prefix
                    // that is known to be larger than s_double[i...].
                    // The next candidate for j should be j + k + 1.
                    j = j + k + 1;
                } else { // s_double[i + k] > s_double[j + k]
                    // The rotation starting at j is smaller.
                    // Similar logic, advance i past all positions that start with a prefix
                    // that is known to be larger than s_double[j...].
                    // The next candidate for i should be i + k + 1.
                    i = i + k + 1;
                }
                k = 0; // Reset common prefix length as we've advanced a pointer.
            }

            // Ensure i and j are distinct. If they become equal, advance j.
            // This is crucial because we are comparing two distinct candidate rotations.
            // If i and j point to the same start, they represent the same rotation,
            // and we need a second distinct candidate.
            if (i == j) {
                j++;
            }
        }

        // After the loop, one of i or j (whichever is smaller and still within [0, N-1])
        // will point to the start of the lexicographically smallest rotation.
        // The algorithm guarantees that the minimum of the final i and j (before they potentially
        // went out of bounds) will be the correct starting index.
        int start_index = std::min(i, j);

        // Return the substring of length N starting at start_index from s_double.
        // This substring is the lexicographically smallest rotation.
        return s_double.substr(start_index, n);
    }
};