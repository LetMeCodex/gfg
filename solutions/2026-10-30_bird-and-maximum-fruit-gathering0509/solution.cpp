#include <vector>    // Required for std::vector
#include <algorithm> // Required for std::min and std::max

class Solution {
public:
    // Function to find the maximum number of packages that can be delivered
    // within a given totalTime.
    // arr: A vector of integers where arr[i] is the number of packages at location i.
    // totalTime: The maximum time available for delivery.
    // Returns: The maximum number of packages delivered.
    int maxPackages(std::vector<int>& arr, int totalTime) {
        int n = arr.size(); // Get the total number of locations.

        // The number of consecutive locations the agent can visit is limited by:
        // 1. The total time available (`totalTime`).
        // 2. The total number of locations available (`n`).
        // So, the effective window size `k` is the minimum of these two values.
        // Constraints state 1 <= arr.size() and 1 <= totalTime,
        // which guarantees n >= 1 and totalTime >= 1, so k will always be >= 1.
        int k = std::min(totalTime, n);

        // `current_sum` will store the sum of packages in the current sliding window.
        // `max_sum` will store the maximum sum found across all possible windows.
        // The maximum possible sum (10^5 locations * 10^3 packages/location = 10^8)
        // fits within a standard 32-bit signed integer.
        int current_sum = 0;
        
        // Step 1: Calculate the sum of packages for the first window of size `k`.
        // This window covers locations from index 0 to k-1.
        for (int i = 0; i < k; ++i) {
            current_sum += arr[i];
        }

        // Initialize `max_sum` with the sum of the first window.
        int max_sum = current_sum;

        // Step 2: Slide the window across the rest of the array.
        // The loop starts from index `k`. This `i` represents the element
        // that is about to enter the window from the right.
        // The loop continues until `i` reaches `n`, meaning the last possible
        // window (ending at `n-1`) has been processed.
        for (int i = k; i < n; ++i) {
            // When the window slides one position to the right:
            // 1. The element at `i - k` (which is the leftmost element of the
            //    previous window) is no longer part of the window. Subtract its value.
            current_sum -= arr[i - k];
            
            // 2. The element at `i` (the new rightmost element) enters the window.
            //    Add its value to `current_sum`.
            current_sum += arr[i];
            
            // 3. Update `max_sum` if the `current_sum` is greater than the
            //    maximum sum found so far.
            max_sum = std::max(max_sum, current_sum);
        }

        // Return the maximum number of packages found across all valid windows.
        return max_sum;
    }
};