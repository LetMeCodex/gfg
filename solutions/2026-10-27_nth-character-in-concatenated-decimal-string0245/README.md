# [Nth in Concatenated Decimal String](https://www.geeksforgeeks.org/problems/nth-character-in-concatenated-decimal-string0245/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Medium
- **Solved In**: 2 attempt(s)

## Problem Summary

The problem asks us to find the Nth character (1-based indexing) in an infinitely concatenated string of positive integers: "123456789101112131415161718192021...".

## Intuition & Approach

The core idea is to efficiently determine which number contains the Nth character and then which digit within that number is the Nth character. We can achieve this by analyzing the structure of the concatenated string based on the number of digits in the integers.

Let's break down the string into "blocks" based on the number of digits:

1.  **1-digit numbers (1-9):**
    *   There are 9 such numbers.
    *   Each number contributes 1 digit to the string.
    *   Total characters contributed: $9 \times 1 = 9$.
    *   The string segment is "123456789".

2.  **2-digit numbers (10-99):**
    *   There are 90 such numbers ($99 - 10 + 1$).
    *   Each number contributes 2 digits to the string.
    *   Total characters contributed: $90 \times 2 = 180$.
    *   The string segment is "101112...9899".

3.  **3-digit numbers (100-999):**
    *   There are 900 such numbers ($999 - 100 + 1$).
    *   Each number contributes 3 digits to the string.
    *   Total characters contributed: $900 \times 3 = 2700$.
    *   The string segment is "100101102...998999".

In general, for numbers with `k` digits:
*   The count of `k`-digit numbers is $9 \times 10^{k-1}$.
*   Each `k`-digit number contributes `k` digits to the string.
*   The total characters contributed by all `k`-digit numbers is $(9 \times 10^{k-1}) \times k$.

The maximum value of `n` is $10^9$. We can estimate the maximum number of digits a number can have to contain the $10^9$-th character:
*   1-digit numbers: 9 characters
*   2-digit numbers: 180 characters (Total: 189)
*   3-digit numbers: 2700 characters (Total: 2889)
*   ...
*   8-digit numbers: $9 \times 10^7 \times 8 = 720,000,000$ characters (Total: approx. 788,888,889)
*   9-digit numbers: $9 \times 10^8 \times 9 = 8,100,000,000$ characters.

Since $10^9$ is less than the total characters contributed by 9-digit numbers, the Nth character will be part of a number with at most 9 digits. This means we can use a loop to find the correct block of numbers (based on digit count) that contains the Nth character.

**Algorithm:**

1.  **Find the Block:**
    *   Start with `digits = 1`, `count = 9` (number of 1-digit numbers), and `power_of_10 = 1` (the first 1-digit number).
    *   Iteratively check if `n` is greater than the total characters contributed by the current block (`digits * count`).
    *   If `n` is greater, subtract `digits * count` from `n`, and update `digits`, `count`, and `power_of_10` to move to the next block (e.g., from 1-digit to 2-digit numbers).
    *   Continue this until `n` is no longer greater than `digits * count`. At this point, `n` represents the 1-based index of the character *within* the current block of `digits`-digit numbers.

2.  **Find the Target Number:**
    *   Once the correct block is identified, `n` is the 1-based index within this block.
    *   The index of the specific number within this block (0-based) can be found by `(n - 1) / digits`. For example, if `n=3` and `digits=2`, it means the 3rd character in the 2-digit block. `(3-1)/2 = 1`. This means it's the 1st number (0-indexed) in the 2-digit block, which is 10.
    *   The actual target number is `power_of_10 + number_index_in_block`.

3.  **Find the Target Character:**
    *   The index of the character within the `target_number` (0-based) can be found by `(n - 1) % digits`. For example, if `n=3` and `digits=2`, `(3-1)%2 = 0`. This means it's the 0th character of the target number.
    *   Convert the `target_number` to a string.
    *   Return the character at `char_index_in_number`.

**Important Considerations:**

*   **1-based vs. 0-based indexing:** The problem uses 1-based indexing for `n`. We need to be careful when converting to 0-based indices for calculations and string access.
*   **Data Types:** Since `n` can be up to $10^9$, and intermediate calculations like `digits * count` can exceed the capacity of a 32-bit integer, it's crucial to use `long long` for `n` and all related variables to prevent overflow.

## Complexity Analysis

*   **Time Complexity**: $O(\log_{10} N)$. The loop to find the correct block runs at most 9-10 times because the number of digits grows logarithmically with `n`. The operations inside the loop and after the loop (string conversion and character access) are constant time relative to the number of digits, which is small.
*   **Space Complexity**: $O(\log_{10} N)$. This is due to the space required to store the string representation of the `target_number`. The length of this string is equal to the number of digits, which is logarithmic with respect to `n`.

## Solution Code

```cpp
#include <string>
#include <algorithm> // Required for std::to_string

// The problem asks for the Nth character (1-based indexing) in a concatenated string
// of decimal representations of positive integers: "123456789101112..."

// We can solve this by first determining which "block" of numbers the Nth character falls into.
// Blocks are defined by the number of digits:
// Block 1: 1-digit numbers (1-9)
// Block 2: 2-digit numbers (10-99)
// Block 3: 3-digit numbers (100-999)
// ... and so on.

// Let's analyze the contribution of each block to the total string length:
// - 1-digit numbers (1 to 9): There are 9 numbers. Each has 1 digit. Total: 9 * 1 = 9 characters.
// - 2-digit numbers (10 to 99): There are 90 numbers. Each has 2 digits. Total: 90 * 2 = 180 characters.
// - 3-digit numbers (100 to 999): There are 900 numbers. Each has 3 digits. Total: 900 * 3 = 2700 characters.
// - In general, for k-digit numbers:
//   - Number of k-digit numbers: 9 * 10^(k-1)
//   - Each number has k digits.
//   - Total characters contributed by k-digit numbers: (9 * 10^(k-1)) * k

// The maximum value of n is 10^9.
// Let's estimate the maximum number of digits needed:
// k=1: 9 chars
// k=2: 180 chars (total 189)
// k=3: 2700 chars (total 2889)
// k=4: 36000 chars (total 38889)
// k=5: 450000 chars (total 488889)
// k=6: 5400000 chars (total 5888889)
// k=7: 63000000 chars (total 68888889)
// k=8: 720000000 chars (total 788888889)
// k=9: 8100000000 chars (total > 10^9)
// So, n will fall into a block of numbers with at most 9 digits.
// The loop to find the block will run at most 9-10 times, making it very efficient.

class Solution {
public:
    // The compilation error indicates that the method name in the driver code is 'nthCharacter'
    // but the implemented method is 'findNthChar'. We need to rename the method to match.
    char nthCharacter(int n_int) {
        // Use long long for 'n' and other intermediate calculations to prevent overflow,
        // as n can be up to 10^9 and intermediate products like 'digits * count' can exceed 2^31-1.
        long long n = n_int; 
        
        long long digits = 1;        // Current number of digits (e.g., 1 for 1-digit numbers, 2 for 2-digit numbers)
        long long count = 9;         // Number of integers with 'digits' digits (e.g., 9 for 1-digit, 90 for 2-digit)
        long long power_of_10 = 1;   // The first number with 'digits' digits (e.g., 1, 10, 100, etc.)

        // This loop determines which block of numbers (1-digit, 2-digit, etc.) the Nth character belongs to.
        // It iteratively subtracts the total characters contributed by previous blocks.
        while (n > digits * count) {
            n -= digits * count;     // Subtract the characters from the current block
            digits++;                // Move to the next number of digits (e.g., from 1-digit to 2-digit)
            count *= 10;             // Update count for the new digit length (e.g., 9 -> 90 -> 900)
            power_of_10 *= 10;       // Update power_of_10 (e.g., 1 -> 10 -> 100)
        }

        // After the loop:
        // - 'n' is now the 1-based index of the character within the current block of 'digits'-digit numbers.
        // - 'digits' is the number of digits of the target number.
        // - 'power_of_10' is the first number in this block (e.g., 1 for 1-digit, 10 for 2-digit, 100 for 3-digit).

        // Calculate the 0-based index of the specific number within its block.
        // For example, if n=1 and digits=2, it's the 0th 2-digit number (which is 10).
        // If n=2 and digits=2, it's still the 0th 2-digit number (10), but the 2nd char.
        // If n=3 and digits=2, it's the 1st 2-digit number (11).
        long long number_index_in_block = (n - 1) / digits;
        
        // Calculate the 0-based index of the character within that specific number.
        // For example, if n=1 and digits=2, char_index_in_number = 0 (first char of 10, which is '1').
        // If n=2 and digits=2, char_index_in_number = 1 (second char of 10, which is '0').
        long long char_index_in_number = (n - 1) % digits;

        // Determine the actual number that contains the Nth character.
        // This is the starting number of the block (power_of_10) plus its index within the block.
        long long target_number = power_of_10 + number_index_in_block;

        // Convert the target number to a string to easily access its characters.
        std::string s_target_number = std::to_string(target_number);

        // Return the character at the calculated index.
        return s_target_number[char_index_in_number];
    }
};
```