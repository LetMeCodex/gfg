#include <vector>
#include <limits> // Not strictly needed if using arr.size() for initialization, but good for general practice.
#include <unordered_map> // Using unordered_map for potentially sparse large values

class Solution {
public:
    // The error message indicates that the main function is calling a method
    // named 'firstElementKTime', but the provided Solution class has a method
    // named 'atLeastKoccurrence'. The method name needs to be corrected to match
    // what the driver code expects.
    int firstElementKTime(std::vector<int>& arr, int k) {
        // Constraints:
        // 1 <= arr.size() <= 10^6
        // 0 <= arr[i] <= 10^6
        // 1 <= k <= 10^3

        // We need to find the first element whose k-th occurrence appears earliest.
        // This implies tracking counts of each element and the index
        // at which its k-th occurrence happens.

        // The original approach used a vector as a frequency map. While this is efficient
        // if the range of values is dense and small, for sparse large values,
        // an unordered_map is more memory-efficient and still provides O(1) average time complexity.
        // Given arr[i] can be up to 10^6, a vector of size 10^6+1 is fine, but if the problem
        // constraints allowed for much larger values (e.g., 10^9), an unordered_map would be necessary.
        // For this specific problem, the vector approach is also valid and might even be slightly faster
        // due to cache locality. However, to demonstrate a more general approach for potentially sparse large values,
        // we'll use an unordered_map.

        std::unordered_map<int, int> counts; // Stores frequency of each element.
        // We also need to store the index of the k-th occurrence for each element.
        // An unordered_map can store pairs or we can use a separate map.
        // Let's use a separate map to store the index of the k-th occurrence.
        std::unordered_map<int, int> kth_occurrence_index;

        int result_element = -1;
        // We need to find the *earliest* k-th occurrence.
        // So, we'll store the index of the k-th occurrence for the current best candidate.
        // Initialize with a value larger than any possible array index.
        // arr.size() is a good choice because valid indices are 0 to arr.size() - 1.
        int min_kth_occurrence_index = arr.size();

        for (int i = 0; i < arr.size(); ++i) {
            int current_element = arr[i];
            counts[current_element]++; // Increment count for the current element.

            // If the count for this element just reached k, it's a candidate.
            // We only care about the *first* time its count reaches k.
            // If its count goes beyond k, it doesn't change its k-th occurrence index.
            if (counts[current_element] == k) {
                // This element's k-th occurrence just happened at index 'i'.
                // Store this index.
                kth_occurrence_index[current_element] = i;

                // Check if this is earlier than the current earliest k-th occurrence.
                if (i < min_kth_occurrence_index) {
                    min_kth_occurrence_index = i;
                    result_element = current_element;
                }
            }
        }

        // If min_kth_occurrence_index is still arr.size(), it means no element
        // reached k occurrences.
        if (min_kth_occurrence_index == arr.size()) {
            return -1;
        }

        return result_element;
    }
};