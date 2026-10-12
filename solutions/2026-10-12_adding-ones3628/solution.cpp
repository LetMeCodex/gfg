#include <vector>
#include <numeric> // Not strictly needed for this solution, but often useful.

class Solution {
public:
    std::vector<int> update(int n, std::vector<int>& updates) {
        // arr will store the final result, initialized to zeros.
        std::vector<int> arr(n, 0);

        // diff array for efficient range updates.
        // Size n+1 to handle updates that affect elements up to index n-1 (0-based).
        // diff[i] stores the net change at index i.
        std::vector<int> diff(n + 1, 0);

        // Process each update.
        // For each value 'j' in updates[] (1-based):
        // Add 1 to arr[i] for all i >= j (1-based).
        // In 0-based indexing, this means adding 1 to arr[i] for i >= j-1.
        // The range affected is [j-1, n-1] (0-based).
        for (int update_idx : updates) {
            // Increment at the start of the range (0-based index).
            // update_idx is 1-based, so j-1 is update_idx - 1.
            diff[update_idx - 1]++;

            // Decrement at the index *after* the end of the range.
            // The range ends at n-1. The index after is n.
            // This ensures the increment effect stops at arr[n-1].
            // The diff array has size n+1, so index n is valid.
            diff[n]--;
        }

        // Calculate prefix sums to get the final array elements.
        // current_add accumulates the net change up to the current index.
        int current_add = 0;
        for (int i = 0; i < n; ++i) {
            current_add += diff[i];
            arr[i] = current_add;
        }

        return arr;
    }
};