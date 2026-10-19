#include <vector>
#include <algorithm> // Required for std::sort
#include <functional> // Required for std::greater<int>

class Solution {
public:
    // Renamed function to match the driver code's expectation.
    std::vector<int> lexicographicallyLargest(std::vector<int>& arr) {
        int n = arr.size();
        
        // Handle edge cases for small arrays.
        // An array with 0 or 1 element cannot have adjacent swaps,
        // and is already lexicographically largest.
        if (n <= 1) {
            return arr;
        }

        // current_block_start_index marks the beginning of the current contiguous
        // block of numbers with the same parity.
        int current_block_start_index = 0;

        // Iterate through the array to find boundaries of same-parity blocks.
        // We iterate up to n-1 because we compare arr[i] with arr[i+1].
        for (int i = 0; i < n - 1; ++i) {
            // Check if arr[i] and arr[i+1] have different parities.
            // (arr[i] % 2) gives 0 for even, 1 for odd (since arr[i] >= 0).
            if ((arr[i] % 2) != (arr[i+1] % 2)) {
                // If parities are different, it means the current block of
                // same-parity numbers ends at arr[i].
                // To make the array lexicographically largest, we sort this block
                // in descending order.
                // std::sort operates on the range [first, last).
                std::sort(arr.begin() + current_block_start_index, 
                          arr.begin() + i + 1, // The end iterator is exclusive
                          std::greater<int>());
                
                // The next block of same-parity numbers starts from arr[i+1].
                current_block_start_index = i + 1;
            }
        }

        // After the loop, there will always be one last block of same-parity numbers
        // from current_block_start_index to the end of the array (n-1).
        // Sort this final block in descending order as well.
        std::sort(arr.begin() + current_block_start_index, 
                  arr.end(), // arr.end() points one past the last element
                  std::greater<int>());

        // Return the modified array.
        return arr;
    }
};