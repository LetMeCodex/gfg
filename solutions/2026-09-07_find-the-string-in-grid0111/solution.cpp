#include <vector>
#include <string>
#include <algorithm>
#include <set> // Using set to handle uniqueness automatically

using namespace std;

class Solution {
public:
    // Modified to match the call in Driver.cpp
    vector<vector<int>> searchWord(vector<vector<char>>& mat, string word) {
        // Derive n and m from the input matrix
        int n = mat.size();
        if (n == 0) {
            return {}; // Handle empty matrix case
        }
        int m = mat[0].size();
        if (m == 0) {
            return {}; // Handle empty rows case
        }

        // Using a set to store unique coordinates and automatically handle lexicographical order
        set<vector<int>> unique_results;
        int wordLen = word.length();

        // Define the 8 possible directions (horizontal, vertical, diagonal)
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        // Iterate through each cell of the grid as a potential starting point
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                // If the current cell matches the first character of the word
                if (mat[i][j] == word[0]) {
                    // Try all 8 directions from this starting cell
                    for (int dir = 0; dir < 8; ++dir) {
                        int k;
                        int current_x = i;
                        int current_y = j;
                        bool found_in_direction = true; // Flag to track if word is found in current direction

                        // Check if the rest of the word can be formed in this direction
                        for (k = 1; k < wordLen; ++k) {
                            current_x += dx[dir];
                            current_y += dy[dir];

                            // Check for boundary conditions and character mismatch
                            if (current_x < 0 || current_x >= n || current_y < 0 || current_y >= m || mat[current_x][current_y] != word[k]) {
                                found_in_direction = false; // Word cannot be formed in this direction
                                break;
                            }
                        }

                        // If the entire word was found in this direction
                        if (found_in_direction) {
                            unique_results.insert({i, j}); // Add the starting coordinates to the set
                        }
                    }
                }
            }
        }

        // Convert the set to a vector for the final result
        vector<vector<int>> result(unique_results.begin(), unique_results.end());
        return result;
    }
};