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