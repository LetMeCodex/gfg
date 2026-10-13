# Roll Characters of String

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to take a string `s` and a vector of integers `roll`. Each integer `r` in `roll` represents a "roll" operation that needs to be applied to the first `r` characters of the string. A "roll" operation means shifting each of the first `r` characters one position forward in the alphabet, with 'z' wrapping around to 'a'. We need to perform all these roll operations and return the modified string.

## Intuition & Approach

The core challenge of this problem lies in efficiently handling multiple overlapping "roll" operations. A naive approach would be to iterate through each roll operation and then iterate through the first `r` characters of the string to perform the shift. However, if `roll` contains many large values, this can lead to a very high time complexity.

Consider the effect of a single roll operation of length `r`. It increments the "roll count" for all characters from index 0 to `r-1`. If we have multiple roll operations, the total shift applied to a character at index `i` is the sum of all roll counts for operations whose length `r` is greater than `i`.

This pattern of applying an update to a range of elements and then querying the cumulative effect at each element suggests using a **difference array**.

Here's how the difference array approach works:

1.  **Difference Array Initialization**: We create a difference array, let's call it `diff`, of size `n + 1` (where `n` is the length of the string `s`), initialized with zeros. The `diff` array will store the *net change* in the number of active roll operations at each index. `diff[i]` will represent how many roll operations *start* at index `i`.

2.  **Processing Roll Operations**: For each roll operation `r` in the `roll` vector:
    *   This operation affects characters from index 0 up to `r-1`.
    *   We increment `diff[0]` by 1. This signifies that a new roll operation begins at index 0.
    *   If `r` is less than `n`, it means the roll operation stops affecting characters at index `r`. To counteract the effect of this roll operation for indices `r` and beyond, we decrement `diff[r]` by 1. This is the standard difference array technique for range updates: `diff[start]++` and `diff[end+1]--`. In our case, the range is `[0, r-1]`, so `start = 0` and `end = r-1`. Thus, we do `diff[0]++` and `diff[r]--` (if `r < n`).

3.  **Calculating Cumulative Roll Count**: After processing all roll operations, we iterate through the string from index 0 to `n-1`. We maintain a `current_roll_count`.
    *   At each index `i`, we update `current_roll_count` by adding `diff[i]` to it. This `current_roll_count` now represents the total number of roll operations that are active and affect the character at index `i`.
    *   We then take the original character `s[i]`, convert it to its numerical value (e.g., 'a' -> 0, 'b' -> 1), add the `current_roll_count` to it, and take the result modulo 26 to handle the wrap-around.
    *   Finally, we convert this new numerical value back to a character and update `s[i]`.

This approach ensures that each roll operation is processed in O(1) time (for updating the difference array), and then we iterate through the string once to apply the cumulative shifts.

The reason for 2 attempts might be:
*   **Attempt 1**: A naive approach that is too slow.
*   **Attempt 2**: Realizing the need for an efficient range update mechanism and implementing the difference array. A common mistake could be off-by-one errors in the difference array indexing or the modulo operation.

## Complexity Analysis

-   **Time Complexity**: $O(N + M)$, where $N$ is the length of the string `s` and $M$ is the number of roll operations (i.e., `roll.size()`).
    *   Initializing the difference array takes $O(N)$ time.
    *   Processing each of the $M$ roll operations takes $O(1)$ time to update the difference array. So, this step is $O(M)$.
    *   Iterating through the string to calculate cumulative roll counts and update characters takes $O(N)$ time.
    *   Therefore, the total time complexity is $O(N + M)$.

-   **Space Complexity**: $O(N)$, where $N$ is the length of the string `s`.
    *   We use a difference array `diff` of size $N+1$, which takes $O(N)$ space.
    *   The string `s` itself takes $O(N)$ space, but we are modifying it in-place, so it's often not counted as extra space. If we consider the output string as extra space, it's still $O(N)$.

## Solution Code

```cpp
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
```