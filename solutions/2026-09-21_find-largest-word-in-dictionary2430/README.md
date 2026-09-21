# [Longest Matching in Dictionary with Removals](https://www.geeksforgeeks.org/problems/find-largest-word-in-dictionary2430/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 3 attempt(s)

## Problem Summary
Given a string `s` and a dictionary `d` (a list of strings), find the longest string in the dictionary that can be formed by deleting zero or more characters from `s`. If there are multiple strings with the same maximum length, return the one that is lexicographically smallest.

## Intuition & Approach
To solve this problem efficiently, we need to prioritize two criteria: **length** (descending) and **lexicographical order** (ascending).

1.  **Sorting**: By sorting the dictionary first, we ensure that the first word we successfully match against `s` is our optimal answer. We define a custom comparator:
    *   If lengths are different, the longer string comes first.
    *   If lengths are equal, the lexicographically smaller string comes first.
2.  **Two-Pointer Matching**: For each word in the sorted dictionary, we check if it is a "subsequence" of `s`. We use two pointers:
    *   `i` to traverse the source string `s`.
    *   `j` to traverse the current dictionary word.
    *   If `s[i] == word[j]`, we increment `j`. We always increment `i`.
3.  **Early Exit**: If `j` reaches the length of the current word, it means all characters were found in `s` in the correct relative order. Since the dictionary is sorted, this is guaranteed to be the best possible result.

## Complexity Analysis
- **Time Complexity**: $O(N \cdot L \cdot \log N + N \cdot M)$, where $N$ is the number of words in the dictionary, $L$ is the average length of a word, and $M$ is the length of string `s`.
    *   Sorting takes $O(N \log N \cdot L)$.
    *   The two-pointer check takes $O(N \cdot M)$.
- **Space Complexity**: $O(1)$ (excluding the space required for sorting, which depends on the implementation).

## Solution Code

```cpp
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

            // Two-pointer check to see if 'word' is a subsequence of 's'
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
```