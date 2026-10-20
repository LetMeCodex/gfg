#include <vector>
#include <algorithm> // Required for std::min

class Solution {
public:
    // The GeeksforGeeks judge expects a method named 'minSubarraySum'.
    // The previous attempt had 'minimumSumSubarray', leading to a compilation error.
    int minSubarraySum(std::vector<int>& arr) {
        // Constraints: 1 <= arr.size() <= 10^5
        // This means 'arr' will always contain at least one element,
        // so there's no need to handle an empty array case.

        // Initialize min_so_far and current_min with the first element of the array.
        // This correctly handles arrays with a single element and sets up the
        // initial state for the iteration.
        int min_so_far = arr[0];
        int current_min = arr[0];

        // Iterate through the array starting from the second element.
        // The loop variable 'i' is of type size_t, which is appropriate for vector indices.
        for (size_t i = 1; i < arr.size(); ++i) {
            // For each element arr[i], we decide whether to extend the current minimum
            // subarray or start a new one from arr[i].
            // We want the minimum of:
            // 1. arr[i] itself (starting a new subarray)
            // 2. current_min + arr[i] (extending the previous minimum subarray)
            // This is the core logic for finding the minimum sum subarray,
            // analogous to Kadane's algorithm for maximum sum.
            current_min = std::min(arr[i], current_min + arr[i]);

            // After updating current_min, we compare it with min_so_far to keep track
            // of the overall minimum sum found across all subarrays encountered so far.
            min_so_far = std::min(min_so_far, current_min);
        }

        // After iterating through all elements, min_so_far will hold the
        // minimum sum of any contiguous subarray.
        return min_so_far;
    }
};