#include <vector>
#include <algorithm> // Required for std::sort and std::max

class Solution {
public:
    // Function to find the maximum possible frequency of any element
    // after performing at most k operations.
    // arr: The input integer array.
    // k: The maximum number of increment operations allowed.
    int maxFrequency(std::vector<int>& arr, int k) {
        // Step 1: Sort the array.
        // Sorting is crucial because if we want to make a subarray
        // [arr[left], ..., arr[right]] all equal to some value,
        // the optimal target value is arr[right] (the largest element in the window).
        // This minimizes the total increments needed.
        std::sort(arr.begin(), arr.end());

        int n = arr.size();
        long long current_sum = 0; // Stores the sum of elements in the current window [left, right].
                                   // Use long long to prevent potential overflow, as sum can be up to 10^5 * 10^6 = 10^11.
        int max_frequency = 0;     // Stores the maximum frequency found so far.
        int left = 0;              // Left pointer of the sliding window.

        // Step 2: Use a sliding window approach.
        // The 'right' pointer expands the window.
        for (int right = 0; right < n; ++right) {
            current_sum += arr[right]; // Add the current element to the window sum.

            // Calculate the cost to make all elements in the current window [left, right]
            // equal to arr[right].
            // The cost is (number_of_elements * target_value) - sum_of_elements.
            // Here, number_of_elements is (right - left + 1), and target_value is arr[right].
            long long window_length = right - left + 1;
            long long cost = window_length * arr[right] - current_sum;

            // If the calculated cost exceeds the allowed operations 'k',
            // we need to shrink the window from the left.
            // We remove elements from the left until the cost condition is met.
            while (cost > k) {
                current_sum -= arr[left]; // Remove arr[left] from the sum.
                left++;                   // Move the left pointer to the right.
                
                // Recalculate window_length and cost for the new, smaller window.
                // The target value (arr[right]) remains the same as we are only shrinking from left.
                window_length = right - left + 1;
                cost = window_length * arr[right] - current_sum;
            }

            // At this point, the current window [left, right] is valid, meaning
            // all its elements can be made equal to arr[right] with at most 'k' operations.
            // Update the maximum frequency found.
            // window_length is at most N (10^5), so it fits in an int.
            max_frequency = std::max(max_frequency, (int)window_length);
        }

        // The problem guarantees arr.size() >= 1, so max_frequency will be at least 1.
        return max_frequency;
    }
};