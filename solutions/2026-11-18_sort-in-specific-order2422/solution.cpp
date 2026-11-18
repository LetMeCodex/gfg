#include <vector>
#include <algorithm>
#include <functional> // Required for std::greater

class Solution {
public:
    std::vector<int> sortIt(std::vector<int>& arr) {
        // Partition the array into odd numbers (true) and even numbers (false)
        // The elements for which the predicate returns true will be moved to the beginning.
        // We want odd numbers first, so the predicate should return true for odd numbers.
        auto partition_point = std::partition(arr.begin(), arr.end(), [](int x){
            return x % 2 != 0; // True for odd numbers
        });

        // Now, arr is partitioned such that:
        // [odd_numbers..., even_numbers...]

        // Sort the odd numbers (from arr.begin() to partition_point) in descending order
        std::sort(arr.begin(), partition_point, std::greater<int>());

        // Sort the even numbers (from partition_point to arr.end()) in ascending order
        std::sort(partition_point, arr.end());

        return arr; // Return the modified array
    }
};