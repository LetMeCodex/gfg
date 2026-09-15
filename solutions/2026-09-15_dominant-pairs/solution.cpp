#include <vector>
#include <algorithm>

class Solution {
public:
    /**
     * @brief Counts the number of dominant pairs in an even-sized integer array.
     *
     * A pair of indices (i, j) is dominant if:
     * 0 <= i < arr.size() / 2
     * arr.size() / 2 <= j < arr.size()
     * arr[i] >= 5 * arr[j]
     *
     * @param arr The input even-sized integer array.
     * @return The total number of dominant pairs.
     */
    int dominantPairs(std::vector<int>& arr) {
        int n = arr.size();
        int mid = n / 2;
        int count = 0;

        // Extract the second half and sort it
        std::vector<int> second_half;
        second_half.reserve(n - mid);
        for (int j = mid; j < n; ++j) {
            second_half.push_back(arr[j]);
        }
        std::sort(second_half.begin(), second_half.end());

        // Iterate through the first half
        for (int i = 0; i < mid; ++i) {
            // The condition is arr[i] >= 5 * arr[j]
            // Which is equivalent to arr[j] <= arr[i] / 5.0
            // We use double to handle negative division correctly.
            double threshold = (double)arr[i] / 5.0;

            // Find the number of elements in second_half that are <= threshold.
            // upper_bound returns the first element > threshold.
            auto it = std::upper_bound(second_half.begin(), second_half.end(), threshold);
            
            count += (int)std::distance(second_half.begin(), it);
        }

        return count;
    }
};