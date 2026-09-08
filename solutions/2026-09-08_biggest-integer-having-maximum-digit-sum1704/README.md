# [Max Digit Sum Number in 1 to n](https://www.geeksforgeeks.org/problems/biggest-integer-having-maximum-digit-sum1704/1)

- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: Easy
- **Solved In**: 2 attempt(s)

## Problem Summary
Given an integer $n$, the task is to find the largest integer $x$ in the range $[1, n]$ such that the sum of the digits of $x$ is maximized. If there are multiple such integers, return the largest one among them.

## Intuition & Approach
To maximize the digit sum, we want as many '9's as possible in the number. For any given number $n$, we can generate potential candidates by modifying its digits.

1. **The Strategy**: For each digit position $i$ in the number $n$, we consider a candidate number formed by:
   - Keeping all digits to the left of $i$ the same as $n$.
   - Decrementing the digit at position $i$ by 1.
   - Setting all digits to the right of $i$ to '9'.
   
   *Example*: If $n = 485$, we consider:
   - $i=0$: $399$ (Digit sum: 21)
   - $i=1$: $479$ (Digit sum: 20)
   - $i=2$: $484$ (Digit sum: 16)
   
2. **Comparison**: We calculate the digit sum for each candidate and compare it against the current `max_digit_sum`. If a candidate has a higher digit sum, we update our answer. If the digit sum is equal, we keep the larger number (though the construction logic naturally favors larger numbers).

3. **Edge Cases**: The original number $n$ itself is always a valid candidate, so we initialize our `max_digit_sum` and `ans` with $n$.

## Complexity Analysis
- **Time Complexity**: $O(L^2)$, where $L$ is the number of digits in $n$ (i.e., $L = \log_{10} n$). For each of the $L$ positions, we perform string operations and digit sum calculations that take $O(L)$ time. Given $n$ is typically up to $10^{18}$, $L \approx 18$, making this approach very efficient.
- **Space Complexity**: $O(L)$ to store the string representation of the number and its candidates.

## Solution Code

```cpp
#include <string>
#include <algorithm>
#include <vector>

class Solution {
public:
    long long findMax(long long n) {
        std::string s = std::to_string(n);
        int len = s.length();
        
        // Calculate initial digit sum of n
        long long current_digit_sum = 0;
        for (char c : s) {
            current_digit_sum += (c - '0');
        }
        
        long long max_digit_sum = current_digit_sum;
        long long ans = n;

        // Iterate through each digit position
        for (int i = 0; i < len; ++i) {
            if (s[i] == '0') continue;

            // Construct candidate: decrement current digit, set following to 9
            std::string temp_s = s;
            temp_s[i]--; 
            for (int j = i + 1; j < len; ++j) {
                temp_s[j] = '9';
            }

            long long candidate_num = std::stoll(temp_s);
            
            // Calculate digit sum of the candidate
            long long candidate_digit_sum = 0;
            std::string candidate_str = std::to_string(candidate_num);
            for (char c : candidate_str) {
                candidate_digit_sum += (c - '0');
            }

            // Update result if better sum found or equal sum with larger value
            if (candidate_digit_sum > max_digit_sum) {
                max_digit_sum = candidate_digit_sum;
                ans = candidate_num;
            } else if (candidate_digit_sum == max_digit_sum) {
                ans = std::max(ans, candidate_num);
            }
        }

        return ans;
    }
};
```