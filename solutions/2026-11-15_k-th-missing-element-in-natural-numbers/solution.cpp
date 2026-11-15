#include <vector>
#include <numeric>
#include <algorithm>

class Solution {
public:
    // Renamed from kthMissing to findKthSmall to match the driver code.
    int findKthSmall(std::vector<int>& arr, int k) {
        // Sort the array to easily calculate missing numbers.
        // Although the problem statement says distinct positive integers,
        // sorting is crucial for the logic below.
        std::sort(arr.begin(), arr.end());

        // If k is smaller than the first missing number before arr[0]
        // The number of missing elements before arr[0] is arr[0] - 1.
        if (k <= arr[0] - 1) {
            return k;
        }

        // Adjust k by subtracting the missing numbers before the first element.
        k -= (arr[0] - 1);

        // Iterate through the sorted array to find the k-th missing number.
        for (size_t i = 0; i < arr.size() - 1; ++i) {
            // Calculate the number of missing elements between arr[i] and arr[i+1].
            int missing_count = arr[i+1] - arr[i] - 1;

            // If k falls within this range of missing numbers.
            if (k <= missing_count) {
                // The k-th missing number is arr[i] + k.
                return arr[i] + k;
            }
            // Otherwise, subtract the missing numbers from k and continue.
            k -= missing_count;
        }

        // If k is still greater than 0 after checking all gaps in the array,
        // it means the k-th missing number is beyond the last element of arr.
        // The k-th missing number will be the last element of arr plus the remaining k.
        return arr.back() + k;
    }
};