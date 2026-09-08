#include <string>
#include <algorithm>
#include <vector>
#include <iostream> // Added for potential debugging if needed, though not strictly required by the problem

class Solution {
public:
    // Renamed from maxSumDigit to findMax to match the driver code's call.
    long long findMax(long long n) {
        std::string s = std::to_string(n);
        int len = s.length();
        long long max_digit_sum = 0;
        long long ans = n;

        // Calculate digit sum of n
        long long current_digit_sum = 0;
        for (char c : s) {
            current_digit_sum += (c - '0');
        }
        max_digit_sum = current_digit_sum;
        ans = n;

        // Iterate through each digit position from left to right (most significant to least significant)
        for (int i = 0; i < len; ++i) {
            // If the current digit is '0', we cannot decrement it to form a valid smaller number
            // by just changing this digit and subsequent ones to 9.
            // For example, if n = 100, and i = 1 (digit '0'), decrementing it would lead to '1(-1)9', which is invalid.
            // The logic of setting subsequent digits to '9' is to maximize the sum.
            // If we have a number like 400, we want to consider 399.
            // If we have 480, we want to consider 479.
            // The current loop structure handles this by considering decrementing the current digit.
            if (s[i] == '0') {
                continue;
            }

            // Construct a candidate number by decreasing the current digit by 1
            // and setting all subsequent digits to 9.
            std::string temp_s = s;
            temp_s[i]--; // Decrement the current digit
            for (int j = i + 1; j < len; ++j) {
                temp_s[j] = '9'; // Set all subsequent digits to '9'
            }

            // Convert the string back to a long long.
            // std::stoll can handle leading zeros correctly (e.g., "09" becomes 9).
            long long candidate_num = std::stoll(temp_s);

            // Calculate the digit sum of the candidate number
            long long candidate_digit_sum = 0;
            // We need to recalculate the digit sum from the candidate_num string representation
            // because stoll might have removed leading zeros, but the logic of decrementing
            // and setting to 9 is based on the original string length.
            // For example, if n = 100, and we consider i=0, temp_s becomes "099". stoll("099") is 99.
            // The digit sum of 99 is 18.
            std::string candidate_str = std::to_string(candidate_num); // Re-convert to string to get its actual digits
            for (char c : candidate_str) {
                candidate_digit_sum += (c - '0');
            }

            // Update max_digit_sum and ans if a better candidate is found
            if (candidate_digit_sum > max_digit_sum) {
                max_digit_sum = candidate_digit_sum;
                ans = candidate_num;
            } else if (candidate_digit_sum == max_digit_sum) {
                // If digit sums are equal, choose the larger number.
                ans = std::max(ans, candidate_num);
            }
        }

        return ans;
    }
};