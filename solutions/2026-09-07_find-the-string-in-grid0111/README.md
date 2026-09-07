# [Word in Grid - All Occurrences](https://www.geeksforgeeks.org/problems/find-the-string-in-grid0111/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 4 attempt(s)

## Problem Summary

The problem asks us to find all occurrences of a given `word` within a 2D character `grid`. The word can appear horizontally, vertically, or diagonally in any of the 8 possible directions. We need to return a list of coordinates `[row, col]` for each starting cell where the word is found. The output should be sorted lexicographically.

## Intuition & Approach

The core idea is to iterate through every cell in the grid and, if that cell's character matches the first character of the `word`, explore all 8 possible directions from that cell to see if the rest of the `word` can be formed.

1.  **Grid Traversal**: We'll use nested loops to visit each cell `(i, j)` in the `mat` grid.

2.  **Starting Point Check**: For each cell `(i, j)`, we check if `mat[i][j]` is equal to `word[0]`. If it is, this cell is a potential starting point for the `word`.

3.  **Directional Search**: If `mat[i][j]` matches `word[0]`, we then need to check all 8 possible directions. These directions can be represented by changes in row and column indices:
    *   `dx[] = {-1, -1, -1, 0, 0, 1, 1, 1}`
    *   `dy[] = {-1, 0, 1, -1, 1, -1, 0, 1}`
    These arrays represent the relative movements for:
    *   Up-Left, Up, Up-Right
    *   Left, Right
    *   Down-Left, Down, Down-Right

4.  **Word Formation Check**: For each of the 8 directions, we'll try to match the subsequent characters of the `word`. We maintain `current_x` and `current_y` starting from `(i, j)` and update them based on `dx[dir]` and `dy[dir]` for each character `k` from `1` to `word.length() - 1`.
    *   **Boundary Checks**: At each step, we must ensure that `current_x` and `current_y` remain within the grid boundaries (`0 <= current_x < n` and `0 <= current_y < m`).
    *   **Character Match**: We also check if `mat[current_x][current_y]` matches `word[k]`.
    *   **Early Exit**: If at any point the boundary conditions are violated or the characters don't match, we know the `word` cannot be formed in this particular direction from this starting cell, so we break the inner loop for this direction.

5.  **Storing Results**: If we successfully match all characters of the `word` in a specific direction (i.e., the inner loop completes without breaking), it means we've found an occurrence. We then add the starting coordinates `[i, j]` to our result set.

6.  **Handling Duplicates and Sorting**: The problem requires unique starting coordinates and lexicographical sorting. A `std::set<std::vector<int>>` is an excellent choice here.
    *   It automatically handles uniqueness: inserting a duplicate `vector<int>` will have no effect.
    *   It automatically sorts elements: `std::vector<int>` has a default comparison operator that performs lexicographical comparison, which is exactly what we need.

7.  **Final Output**: After iterating through all cells and all directions, we convert the `set` of unique coordinates into a `std::vector<std::vector<int>>` to match the required return type.

**Self-Correction/Refinement during thought process:**

*   **Initial thought**: Might consider using a `std::vector<std::vector<int>>` and then sorting and removing duplicates at the end.
*   **Refinement**: Realized that `std::set<std::vector<int>>` handles both uniqueness and sorting automatically, simplifying the code and potentially improving efficiency for duplicate handling.
*   **Edge Cases**: Considered empty grid (`n=0` or `m=0`) and handled them by returning an empty result.
*   **Clarity**: Ensured variable names are descriptive (`current_x`, `current_y`, `found_in_direction`).
*   **Driver Code Compatibility**: Modified the function signature to `vector<vector<int>> searchWord(vector<vector<char>>& mat, string word)` to match the expected signature in the `Driver.cpp` file often provided with GeeksforGeeks problems.

## Complexity Analysis

*   **Time Complexity**: $O(N \times M \times 8 \times L)$
    *   $N$ is the number of rows in the grid.
    *   $M$ is the number of columns in the grid.
    *   We iterate through each of the $N \times M$ cells.
    *   From each cell, we explore 8 directions.
    *   In each direction, we potentially traverse up to $L$ characters, where $L$ is the length of the `word`.
    *   The insertion into a `std::set` takes $O(\log K)$ time, where $K$ is the number of elements in the set. In the worst case, $K$ can be up to $N \times M$. However, the dominant factor is the grid traversal and word checking.
    *   Therefore, the overall time complexity is approximately $O(N \times M \times L)$.

*   **Space Complexity**: $O(K)$
    *   $K$ is the number of unique starting positions where the word is found. In the worst case, $K$ can be up to $N \times M$.
    *   The `std::set` stores these coordinates.
    *   The final `result` vector also stores these coordinates.
    *   Therefore, the space complexity is $O(K)$, which is at most $O(N \times M)$.

## Solution Code

```cpp
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
        // dx: change in row, dy: change in column
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
                        // Start from the second character of the word (index 1)
                        for (k = 1; k < wordLen; ++k) {
                            current_x += dx[dir];
                            current_y += dy[dir];

                            // Check for boundary conditions and character mismatch
                            if (current_x < 0 || current_x >= n || current_y < 0 || current_y >= m || mat[current_x][current_y] != word[k]) {
                                found_in_direction = false; // Word cannot be formed in this direction
                                break; // Exit the inner loop for this direction
                            }
                        }

                        // If the entire word was found in this direction (inner loop completed without breaking)
                        if (found_in_direction) {
                            unique_results.insert({i, j}); // Add the starting coordinates to the set
                        }
                    }
                }
            }
        }

        // Convert the set to a vector for the final result
        // The set automatically provides elements in lexicographical order.
        vector<vector<int>> result(unique_results.begin(), unique_results.end());
        return result;
    }
};
```