#include <vector>
#include <map>
#include <algorithm> // Required for std::vector comparison (lexicographical)

class Solution {
public:
    std::vector<int> findPairs(std::vector<int>& arr) {
        int N = arr.size(); // Get the size of the input array.

        // A map to store sums of pairs and the indices of the first pair that produced that sum.
        // Key: The sum of two elements (arr[i] + arr[j]).
        // Value: A pair of integers {i, j} representing the indices of the first pair found.
        // Using std::map ensures elements are ordered by key, but for this problem,
        // the order of keys doesn't directly affect the lexicographical result,
        // only the order of iteration for i and j does. std::unordered_map could also be used
        // for potentially faster average-case performance, but std::map offers guaranteed O(log N)
        // worst-case for operations, which is sufficient here.
        std::map<int, std::pair<int, int>> sum_to_indices;

        // Initialize the result vector with {-1, -1, -1, -1},
        // which signifies that no such quadruplet has been found yet.
        std::vector<int> result = {-1, -1, -1, -1};

        // Iterate through all possible pairs (i, j) such that i < j.
        // This ensures that for any pair (i, j), i and j are distinct and i is always smaller than j.
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                int current_sum = arr[i] + arr[j]; // Calculate the sum of the current pair.

                // Check if this 'current_sum' has been encountered before.
                if (sum_to_indices.count(current_sum)) {
                    // If the sum exists in the map, it means we found a previous pair
                    // (prev_a, prev_b) that also sums to 'current_sum'.
                    int prev_a = sum_to_indices[current_sum].first;
                    int prev_b = sum_to_indices[current_sum].second;

                    // Now we have two pairs: (prev_a, prev_b) and (i, j).
                    // We need to ensure all four indices (prev_a, prev_b, i, j) are distinct.
                    // The conditions prev_a < prev_b and i < j are already guaranteed by
                    // how sum_to_indices is populated and the loop structure.
                    // We only need to check if prev_a, prev_b are distinct from i, j.
                    if (prev_a != i && prev_a != j && prev_b != i && prev_b != j) {
                        // All four indices are distinct. We have found a valid quadruplet.
                        std::vector<int> current_quadruplet;

                        // To satisfy the lexicographical requirement (a < c),
                        // we assign the pair with the smaller first index as (a, b)
                        // and the other pair as (c, d).
                        if (prev_a < i) {
                            current_quadruplet = {prev_a, prev_b, i, j};
                        } else { // i < prev_a (prev_a cannot be equal to i because of the distinctness check)
                            current_quadruplet = {i, j, prev_a, prev_b};
                        }

                        // Compare this newly found quadruplet with the current best 'result'.
                        // std::vector's operator< performs lexicographical comparison,
                        // which is exactly what's needed for the problem.
                        if (result[0] == -1 || current_quadruplet < result) {
                            result = current_quadruplet; // Update result if current_quadruplet is smaller.
                        }
                    }
                } else {
                    // If 'current_sum' is not in the map, this is the first time we've seen it.
                    // Store the current pair (i, j) as the representative for this sum.
                    sum_to_indices[current_sum] = {i, j};
                }
            }
        }

        // After checking all possible pairs, 'result' will hold the lexicographically
        // smallest quadruplet or {-1, -1, -1, -1} if none was found.
        return result;
    }
};