# GeeksforGeeks POTD: Sum of All Digit Subsequences

## Problem Summary

The problem asks us to calculate the sum of all digits across all possible non-empty subsequences of a given string of digits. The result should be returned modulo $10^9 + 7$.

For example, if the input string is "123", the non-empty subsequences are:
"1", "2", "3", "12", "13", "23", "123".

The sum of digits for each subsequence is:
1, 2, 3, (1+2)=3, (1+3)=4, (2+3)=5, (1+2+3)=6.

The total sum of all these digit sums is $1 + 2 + 3 + 3 + 4 + 5 + 6 = 24$.

## Intuition & Approach

Let the given string of digits be $s$ of length $n$. We need to find the sum of digits of all its non-empty subsequences.

Consider a digit at index $i$ in the string $s$, let's say $s[i]$. We need to determine how many non-empty subsequences contain this digit $s[i]$.

A subsequence is formed by deleting zero or more characters from the original string. To form a subsequence that includes $s[i]$, we must select $s[i]$. For the remaining $n-1$ characters in the string (those before $s[i]$ and those after $s[i]$), we have two choices for each: either include it in the subsequence or exclude it. This gives us $2^{n-1}$ ways to form subsequences that include $s[i]$.

However, this count $2^{n-1}$ includes the subsequence that *only* contains $s[i]$ (when all other $n-1$ characters are excluded). Since we are considering non-empty subsequences, this count is exactly what we need for each digit.

So, each digit $s[i]$ appears in exactly $2^{n-1}$ non-empty subsequences.

The total sum of all digits across all non-empty subsequences can be calculated by summing up the contributions of each digit. The contribution of a digit $s[i]$ to the total sum is its value multiplied by the number of subsequences it appears in.

Therefore, the total sum is:
$\sum_{i=0}^{n-1} (\text{value of } s[i]) \times (\text{number of subsequences } s[i] \text{ appears in})$
$= \sum_{i=0}^{n-1} (\text{value of } s[i]) \times 2^{n-1}$

We can factor out $2^{n-1}$:
$= (\sum_{i=0}^{n-1} \text{value of } s[i]) \times 2^{n-1}$

This means the total sum is simply the sum of all digits in the original string, multiplied by $2^{n-1}$.

Since the problem requires the result modulo $10^9 + 7$, we need to perform all calculations modulo this value.

The steps are:
1. Calculate the sum of all digits in the input string $s$. Let this be `total_digit_sum`.
2. Calculate $2^{n-1}$ modulo $10^9 + 7$. We can use modular exponentiation (binary exponentiation) for this. Let this be `num_occurrences_per_digit`.
3. The final answer is `(total_digit_sum * num_occurrences_per_digit) % MOD`.

**Edge Case:** If the input string is empty ($n=0$), there are no non-empty subsequences, so the sum should be 0. The formula $2^{n-1}$ would involve $2^{-1}$, which is not directly handled. Our `power` function correctly handles $n-1=0$ (for $n=1$, $2^0=1$). For $n=0$, we should return 0.

## Complexity Analysis

- **Time Complexity**: $O(\log N)$
    - Calculating `total_digit_sum`: This involves iterating through the string once, which takes $O(N)$ time, where $N$ is the length of the string.
    - Calculating `power(2, n - 1)`: Modular exponentiation takes $O(\log N)$ time.
    - The dominant part is the iteration through the string. However, if we consider the constraints where $N$ can be up to $10^5$, the $O(N)$ for sum calculation is significant.
    - **Correction**: The problem statement implies that the string `s` consists of digits. The length of `s` is $N$. The sum of digits can be calculated in $O(N)$. The modular exponentiation is $O(\log N)$. Therefore, the overall time complexity is $O(N + \log N)$, which simplifies to $O(N)$.

- **Space Complexity**: $O(1)$
    - We are using a few variables to store intermediate results (`total_digit_sum`, `num_occurrences_per_digit`, `final_sum`, `MOD`). The space used does not depend on the input size $N$.

## Solution Code

```cpp
#include <string> // Required for std::string

class Solution {
public:
    // Modulo constant
    const long long MOD = 1e9 + 7;

    /**
     * @brief Calculates (base^exp) % MOD using modular exponentiation (binary exponentiation).
     * @param base The base of the exponentiation.
     * @param exp The exponent.
     * @return (base^exp) % MOD.
     */
    long long power(long long base, long long exp) {
        long long res = 1;
        base %= MOD; // Ensure base is within modulo range
        while (exp > 0) {
            if (exp % 2 == 1) { // If exp is odd, multiply res by base
                res = (res * base) % MOD;
            }
            base = (base * base) % MOD; // Square the base
            exp /= 2; // Halve the exponent
        }
        return res;
    }

    /**
     * @brief Finds the sum of all digits across all non-empty subsequences of s.
     * @param s The input string consisting of digits.
     * @return The total sum modulo 10^9 + 7.
     */
    int subsequenceSum(std::string s) { // Renamed function to subsequenceSum for clarity
        int n = s.length();

        // Edge case: if string is empty, sum is 0.
        // N=0 would result in N-1 = -1 for power, which is not handled by the power function.
        // For N=0, there are no subsequences, so sum is 0.
        if (n == 0) {
            return 0;
        }

        // Step 1: Calculate the sum of all individual digits in the string s.
        // This sum is kept modulo MOD to prevent overflow.
        long long total_digit_sum = 0;
        for (char c : s) {
            total_digit_sum = (total_digit_sum + (c - '0')) % MOD;
        }

        // Step 2: Calculate the number of times each digit appears in all non-empty subsequences.
        // As derived, each digit s[i] appears in 2^(N-1) subsequences.
        // We need to calculate 2^(N-1) modulo MOD.
        // For N=1, N-1=0, so 2^0=1. The power function handles this correctly.
        long long num_occurrences_per_digit = power(2, n - 1);

        // Step 3: The final sum is the product of (sum of all digits in s) and
        // (number of times each digit appears), all modulo MOD.
        long long final_sum = (total_digit_sum * num_occurrences_per_digit) % MOD;

        return static_cast<int>(final_sum);
    }
};
```