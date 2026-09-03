#include <vector>
#include <numeric>
#include <algorithm>
#include <deque>

class Solution {
public:
    long long maxFruits(std::vector<int>& arr, int m) {
        int n = arr.size();
        if (n == 0) {
            return 0;
        }

        // If m is greater than or equal to n, the bird can visit all trees.
        if (m >= n) {
            long long total_sum = 0;
            for (int fruit : arr) {
                total_sum += fruit;
            }
            return total_sum;
        }

        // Extend the array to handle circularity.
        // We need to consider subarrays of length up to m.
        // The extended array will have size 2*n.
        std::vector<int> extended_arr = arr;
        for (int i = 0; i < n; ++i) {
            extended_arr.push_back(arr[i]);
        }

        // Calculate prefix sums for the extended array.
        // prefix_sum[i] will store the sum of elements from index 0 to i-1 in extended_arr.
        std::vector<long long> prefix_sum(extended_arr.size() + 1, 0);
        for (int i = 0; i < extended_arr.size(); ++i) {
            prefix_sum[i + 1] = prefix_sum[i] + extended_arr[i];
        }

        long long max_fruit = 0;
        std::deque<int> dq; // Stores indices for prefix_sum

        // Iterate through the extended array to find the maximum sum subarray of length at most m.
        // We are looking for max(prefix_sum[j] - prefix_sum[i]) where j - i <= m.
        // This is equivalent to max(prefix_sum[j] - min(prefix_sum[i])) for i in [j-m, j-1].
        // The loop for j goes up to 2*n because the subarray can wrap around.
        for (int j = 0; j <= extended_arr.size(); ++j) {
            // 1. Remove indices from the front of the deque that are outside the window of size m.
            // The window for prefix_sum[i] is [j-m, j-1]. So, we need i >= j-m.
            // If dq.front() < j - m, it's outside the window.
            if (!dq.empty() && dq.front() < j - m) {
                dq.pop_front();
            }

            // 2. Calculate the current maximum fruit if the deque is not empty.
            // The minimum prefix sum in the window is at dq.front().
            // The current sum is prefix_sum[j] - prefix_sum[dq.front()].
            if (!dq.empty()) {
                max_fruit = std::max(max_fruit, prefix_sum[j] - prefix_sum[dq.front()]);
            }

            // 3. Maintain the increasing order of prefix sums in the deque.
            // Remove indices from the back whose prefix sums are greater than or equal to prefix_sum[j].
            // This ensures that dq.front() always points to the minimum prefix sum.
            while (!dq.empty() && prefix_sum[dq.back()] >= prefix_sum[j]) {
                dq.pop_back();
            }

            // 4. Add the current index j to the deque.
            dq.push_back(j);
        }

        return max_fruit;
    }
};