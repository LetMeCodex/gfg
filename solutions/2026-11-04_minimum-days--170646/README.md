# [Minimum Days](https://www.geeksfor geeks.org/problems/minimum-days--170646/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

Given a binary string `s` of length `n` and a 0-indexed permutation array `p` of length `n`.
In one day, you can replace the character at index `p[i]` with a `'?'` character. This means on day 1, `s[p[0]]` is replaced; on day 2, `s[p[1]]` is replaced, and so on.
The task is to find the minimum number of days required such that there are no two adjacent equal characters in the string. A `'?'` character is considered different from any other character. This implies that a violation only occurs if `s[i]` and `s[i+1]` are both non-'?' and `s[i] == s[i+1]`.

**Example:**
`s = "11100"`, `p = [1,0,2,3,4]`
- Initial: `"11100"` (Violations: `s[0]==s[1]`, `s[1]==s[2]`, `s[3]==s[4]`)
- Day 1 (replace `s[p[0]] = s[1]` with `'?'`): `"1?100"` (Violations: `s[3]==s[4]`)
- Day 2 (replace `s[p[1]] = s[0]` with `'?'`): `"?1100"` (Violations: `s[1]==s[2]`, `s[3]==s[4]`) - *Wait, the example in the problem description is different. It says `"?100"` after Day 2. This implies the string is modified cumulatively.*
    - Let's re-read: "In one day, you can replace the character at index `p[i]` with a `'?'` character." This means after `k` days, characters at `p[0], p[1], ..., p[k-1]` are replaced.
    - Example trace:
        - `s = "11100"`, `p = [1,0,2,3,4]`
        - Initial: `"11100"` (Violations: `(0,1)`, `(1,2)`, `(3,4)`)
        - Day 1 (replace `s[p[0]] = s[1]`): `s` becomes `"1?100"` (Violations: `(3,4)`)
        - Day 2 (replace `s[p[1]] = s[0]`): `s` becomes `"?1100"` (Violations: `(1,2)`, `(3,4)`)
        - Day 3 (replace `s[p[2]] = s[2]`): `s` becomes `"?1?00"` (Violations: `(3,4)`)
        - Day 4 (replace `s[p[3]] = s[3]`): `s` becomes `"?1??"0"` (No violations)
        - Minimum days = 4.

The problem asks for the minimum `k` such that after replacing `s[p[0]]` through `s[p[k-1]]` with `'?'`, the condition is met.

## Intuition & Approach

The problem asks for the *minimum* number of days to achieve a certain property. This structure is a strong indicator that binary search can be applied to find the answer.

**Monotonicity:**
Let `k` be the number of days. If we can satisfy the condition (no adjacent equal non-'?' characters) in `k` days, we can certainly satisfy it in `k+1` days (by replacing one more character, which can only help or keep the status quo, never worsen the condition). This monotonic property allows us to use binary search on the number of days `k`.

**Approach:**

1.  **Define Search Space:** The minimum number of days can range from `0` (if the original string already satisfies the condition) to `n` (if all characters need to be replaced to satisfy the condition). So, our search space for `k` is `[0, n]`.

2.  **`check(k)` Function:** We need a helper function, `check(k, n, original_s, p)`, that returns `true` if the condition can be met after `k` days, and `false` otherwise.
    *   Inside `check(k)`:
        *   Create a temporary mutable copy of the `original_s` string.
        *   Apply `k` replacements: For `i` from `0` to `k-1`, replace the character at index `p[i]` in the temporary string with `'?'`.
        *   Iterate through the modified string from `i = 0` to `n-2`.
        *   At each position `i`, check if `current_s[i]` and `current_s[i+1]` are both non-'?' AND they are equal. If such a pair is found, the condition is violated, and `check(k)` returns `false`.
        *   If the loop completes without finding any violations, `check(k)` returns `true`.

3.  **Binary Search:**
    *   Initialize `low = 0`, `high = n`, and `ans = n` (initializing `ans` to `n` ensures that if `0` days is not enough, we have a valid upper bound, and we are looking for the minimum).
    *   While `low <= high`:
        *   Calculate `mid = low + (high - low) / 2`. This way of calculating `mid` prevents potential integer overflow compared to `(low + high) / 2`.
        *   Call `check(mid, n, s, p)`.
        *   If `check(mid)` returns `true`: It means `mid` days are sufficient. We store `mid` as a potential answer (`ans = mid`) and try to find an even smaller number of days by searching in the left half (`high = mid - 1`).
        *   If `check(mid)` returns `false`: It means `mid` days are not enough. We need more days, so we search in the right half (`low = mid + 1`).
    *   Finally, `ans` will hold the minimum number of days required.

This approach efficiently finds the minimum `k` by leveraging the monotonic property of the `check` function.

## Complexity Analysis

*   **Time Complexity**:
    *   The binary search performs `O(log n)` iterations, as it halves the search space `[0, n]` in each step.
    *   In each iteration, the `check` function is called.
        *   Inside `check`:
            *   Copying the string `original_s` to `current_s` takes `O(n)` time.
            *   Applying `k` replacements (where `k` is at most `n`) takes `O(k)` or `O(n)` time.
            *   Iterating through the `current_s` to verify the condition takes `O(n)` time.
        *   Therefore, the `check` function takes `O(n)` time.
    *   Combining these, the total time complexity is `O(log n * n) = O(n log n)`.

*   **Space Complexity**:
    *   The `check` function creates a mutable copy of the input string `s` (`current_s`), which requires `O(n)` space.
    *   The input strings `s` and `p` themselves take `O(n)` space, but this is typically considered input space and not auxiliary space complexity.
    *   Therefore, the auxiliary space complexity is `O(n)`.

## Solution Code

```cpp
#include <string>
#include <vector>

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
        // For each day 'i' from 0 to k-1, replace the character at p[i] with '?'.
        for (int i = 0; i < k; ++i) {
            current_s[p[i]] = '?';
        }
        
        // Check the condition: no two adjacent non-'?' characters are equal.
        // Iterate up to n-1 to check pairs (i, i+1).
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
        // The search space for 'k' is from 0 to n.
        int low = 0;
        int high = n; 
        int ans = n; // Initialize ans to the maximum possible days (n), as we are looking for the minimum.
        
        while (low <= high) {
            int mid = low + (high - low) / 2; // Calculate mid to prevent potential overflow.
            
            if (check(mid, n, s, p)) {
                // If 'mid' days are sufficient, it's a potential answer.
                // Try to find an even smaller number of days in the left half.
                ans = mid;         
                high = mid - 1;    
            } else {
                // If 'mid' days are not sufficient, we need more days.
                // Search in the right half.
                low = mid + 1;     
            }
        }
        
        return ans; // 'ans' will hold the minimum number of days required.
    }
};
```