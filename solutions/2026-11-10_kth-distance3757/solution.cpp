#include <vector>
#include <unordered_map>

class Solution {
public:
    /**
     * Checks if there exist two distinct indices i and j such that arr[i] is equal to arr[j]
     * and the absolute difference between i and j is at most k.
     *
     * @param arr The input array of integers.
     * @param k The maximum allowed distance between indices.
     * @return True if such a pair of indices exists, false otherwise.
     */
    bool checkWithinK(std::vector<int>& arr, int k) { // Renamed function from containsNearbyDuplicate to checkWithinK
        std::unordered_map<int, int> num_to_index;

        for (int i = 0; i < arr.size(); ++i) {
            // If the current number is already in the map
            if (num_to_index.count(arr[i])) {
                // Check if the difference between the current index and the previously seen index
                // is less than or equal to k.
                if (i - num_to_index[arr[i]] <= k) {
                    return true;
                }
            }
            // Update the last seen index for the current number.
            // This is crucial: even if a duplicate was found but its distance was > k,
            // we still need to update its last seen index to the current one,
            // as this new index might form a valid pair with a future occurrence.
            num_to_index[arr[i]] = i;
        }

        // If no such pair is found after iterating through the entire array.
        return false;
    }
};