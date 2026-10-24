# [Sum of Beauty of All Substrings](https://www.geeksforg eeks.org/problems/sum-of-beauty-of-all-substrings-1662962118/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 1 attempt(s)

## Problem Summary

The problem asks us to calculate the "beauty" of all possible substrings of a given string `s` and return the sum of these beauties.

The "beauty" of a substring is defined as the difference between the maximum frequency of any character and the minimum frequency of any character present in that substring. For example, in the substring "aba", 'a' appears twice, 'b' appears once. Max frequency is 2, min frequency is 1. Beauty = 2 - 1 = 1. If a substring has only one unique character (e.g., "aaa"), its max and min frequencies will be the same, resulting in a beauty of 0.

The input string `s` consists of lowercase English letters, and its length `N` can be up to 500.

## Intuition & Approach

### Intuition

The problem requires us to consider *all* possible substrings. This immediately suggests a nested loop structure: one loop to pick the starting character of a substring, and another loop to pick its ending character. For each substring generated this way, we need to calculate the frequencies of its characters to determine the maximum and minimum frequencies. Since the string contains only lowercase English letters, a fixed-size array (of size 26) can efficiently store character frequencies.

### Approach

1.  **Initialize `totalBeauty`**: We'll use an integer variable `totalBeauty` initialized to `0` to accumulate the beauty of all substrings.

2.  **Outer Loop (Starting Position `i`)**: Iterate from `i = 0` to `n-1` (where `n` is the length of the string `s`). This loop fixes the starting character of our current substring.

3.  **Initialize Frequency Array**: Inside the outer loop, for each new starting position `i`, we need a fresh frequency count for characters. Declare a `std::vector<int> freq(26, 0)` to store the frequencies of characters 'a' through 'z' for the substring `s[i...j]`. This `freq` array is reset for every new `i`.

4.  **Inner Loop (Ending Position `j`)**: Iterate from `j = i` to `n-1`. This loop extends the current substring `s[i...j]` one character at a time to the right.

    *   **Update Frequencies**: For each character `s[j]`, increment its count in the `freq` array: `freq[s[j] - 'a']++`.

    *   **Calculate `max_freq` and `min_freq`**: After updating the frequency for `s[j]`, the `freq` array now holds the counts for the substring `s[i...j]`. We need to find the maximum and minimum frequencies among *only those characters that are present* in `s[i...j]`.
        *   Initialize `max_freq = 0`.
        *   Initialize `min_freq = std::numeric_limits<int>::max()`. It's crucial to initialize `min_freq` to a very large value (like `INT_MAX`) because we only want to consider characters that actually appear in the substring (i.e., `freq[k] > 0`). If we initialized it to `0`, and a character was not present, its `freq` would be `0`, incorrectly becoming the minimum.
        *   Iterate through the `freq` array (from `k = 0` to `25`):
            *   If `freq[k] > 0` (meaning character `k` is present in the current substring):
                *   Update `max_freq = std::max(max_freq, freq[k])`.
                *   Update `min_freq = std::min(min_freq, freq[k])`.

    *   **Add to `totalBeauty`**: The beauty of the current substring `s[i...j]` is `(max_freq - min_freq)`. Add this value to `totalBeauty`.

5.  **Return `totalBeauty`**: After both loops complete, `totalBeauty` will hold the sum of beauties of all substrings.

This approach systematically generates all `N * (N+1) / 2` substrings, and for each, it efficiently calculates the required frequencies and beauty.

## Complexity Analysis

*   **Time Complexity**:
    *   The outer loop runs `N` times (for `i`).
    *   The inner loop runs `N-i` times (for `j`). In the worst case, this is `N` times.
    *   Inside the inner loop, we update a character frequency, which is an `O(1)` operation.
    *   Then, we iterate through the `freq` array of size 26 to find `max_freq` and `min_freq`. This is a constant time operation, `O(26)`, which simplifies to `O(1)`.
    *   Therefore, the total time complexity is `N * N * O(1)` (where `O(1)` includes the `O(26)` part). This results in **$O(N^2)$**.
    *   Given `N <= 500`, `N^2 = 250,000`. Multiplying by 26 (for the frequency scan) gives approximately `6.5 * 10^6` operations, which is well within typical time limits for competitive programming (usually `10^8` operations per second).

*   **Space Complexity**:
    *   We use a `std::vector<int> freq(26, 0)` to store character frequencies. The size of this vector is fixed at 26, regardless of the input string length `N`.
    *   Therefore, the space complexity is **$O(1)$** (constant space).

## Solution Code

```cpp
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
```