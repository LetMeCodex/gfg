#include <vector>
#include <algorithm> // Required for std::max

class Solution {
public:
    // Renamed from maxSumWithOneSquare to maximumSubarray to match the driver code.
    long long maximumSubarray(std::vector<int>& arr) {
        int n = arr.size();
        // Constraints state 1 <= arr.size(), so n will always be at least 1.

        // max_so_far_no_square: maximum subarray sum ending at current position,
        //                       where no element in the subarray has been squared yet.
        // max_so_far_one_square: maximum subarray sum ending at current position,
        //                        where exactly one element in the subarray has been squared.
        // overall_max_sum: the global maximum sum found across all subarrays and squaring options.

        // Initialize with the first element of the array.
        // For arr[0]:
        // 1. If arr[0] is part of a subarray with no squared elements: sum is arr[0].
        // 2. If arr[0] is the squared element in a subarray: sum is arr[0]*arr[0].
        long long max_so_far_no_square = arr[0];
        // Cast to long long before multiplication to prevent potential overflow if arr[0]*arr[0]
        // exceeds INT_MAX, though for arr[i] up to 1000, 1000*1000 = 10^6 fits in int.
        long long max_so_far_one_square = (long long)arr[0] * arr[0]; 
        
        // The overall maximum sum must consider both possibilities for the first element.
        long long overall_max_sum = std::max(max_so_far_no_square, max_so_far_one_square);

        // Iterate from the second element to the end of the array.
        for (int i = 1; i < n; ++i) {
            long long current_val = arr[i];
            // current_val is long long, so current_val * current_val will be long long.
            long long current_val_sq = current_val * current_val; 

            // Calculate the new maximum subarray sum ending at 'i' without squaring any element.
            // This can either be 'current_val' itself (starting a new subarray)
            // or 'current_val' added to the previous max_so_far_no_square.
            long long new_max_so_far_no_square = std::max(current_val, max_so_far_no_square + current_val);

            // Calculate the new maximum subarray sum ending at 'i' with exactly one element squared.
            // This can be formed in three ways:
            // 1. Extend a subarray that already had one element squared: max_so_far_one_square + current_val
            //    (current_val is added normally, as one element was already squared before it).
            // 2. Square 'current_val' itself, extending a subarray that previously had no squared elements:
            //    max_so_far_no_square + current_val_sq
            //    (current_val is squared, and added to a sum that had no squared elements before 'arr[i]').
            // 3. Square 'current_val' itself, starting a new subarray: current_val_sq
            //    (current_val is squared, forming a new subarray of just this element).
            long long new_max_so_far_one_square = std::max({max_so_far_one_square + current_val, 
                                                              max_so_far_no_square + current_val_sq, 
                                                              current_val_sq});
            
            // Update the states for the next iteration.
            max_so_far_no_square = new_max_so_far_no_square;
            max_so_far_one_square = new_max_so_far_one_square;

            // Update the overall maximum sum found so far.
            // It could be the current max_so_far_no_square or max_so_far_one_square.
            overall_max_sum = std::max({overall_max_sum, max_so_far_no_square, max_so_far_one_square});
        }

        return overall_max_sum;
    }
};