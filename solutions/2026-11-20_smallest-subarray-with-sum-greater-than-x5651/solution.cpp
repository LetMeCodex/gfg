#include <vector>
#include <algorithm> // Required for std::min

class Solution {
public:
    // Function to find the length of the smallest subarray whose sum is strictly greater than x.
    // Renamed function from 'smallestSubarraySum' to 'smallestSubWithSum'
    // and swapped parameter order to match the GeeksforGeeks driver code.
    int smallestSubWithSum(std::vector<int>& arr, int x) {
        int n = arr.size();
        
        // Initialize two pointers for the sliding window
        int left = 0;
        
        // current_sum can potentially be up to 10^5 * 10^4 = 10^9.
        // A 32-bit signed int can hold up to ~2 * 10^9, so it fits.
        // However, using long long is safer for sums in competitive programming
        // to prevent overflow issues with slightly larger constraints.
        long long current_sum = 0; 
        
        // Initialize min_length to a value greater than any possible subarray length.
        // n+1 is a safe choice, as the maximum possible length is n.
        int min_length = n + 1; 
        
        // Iterate with the right pointer to expand the window
        for (int right = 0; right < n; ++right) {
            // Add the current element to the window sum
            current_sum += arr[right];
            
            // While the current sum is strictly greater than x,
            // we have a candidate subarray. Try to shrink it from the left
            // to find the smallest possible valid subarray ending at 'right'.
            while (current_sum > x) {
                // Update min_length with the current subarray's length
                min_length = std::min(min_length, right - left + 1);
                
                // Shrink the window from the left
                current_sum -= arr[left];
                left++;
            }
        }
        
        // If min_length is still n + 1, it means no subarray was found
        // whose sum is strictly greater than x.
        if (min_length == n + 1) {
            return 0;
        } else {
            return min_length;
        }
    }
};