#include <vector>
#include <algorithm>
#include <climits> // For INT_MAX, INT_MIN

using namespace std;

class Solution {
public:
    int minDiff(int k, vector<int>& arr) {
        int n = arr.size();
        
        // Edge case: If k is equal to n, the entire array is removed.
        // The remaining array is empty, so the difference is 0.
        if (n == k) {
            return 0;
        }
        
        // Problem constraints typically imply 1 <= k <= n.
        // If k=0 were allowed, the logic below would correctly return max(arr) - min(arr).
        // If k > n, it's an invalid input for "remove a subarray of size k".

        // Precompute prefix minimums and maximums
        // prefix_min[j] stores the minimum value in arr[0...j]
        // prefix_max[j] stores the maximum value in arr[0...j]
        vector<int> prefix_min(n);
        vector<int> prefix_max(n);
        prefix_min[0] = arr[0];
        prefix_max[0] = arr[0];
        for (int i = 1; i < n; ++i) {
            prefix_min[i] = min(prefix_min[i-1], arr[i]);
            prefix_max[i] = max(prefix_max[i-1], arr[i]);
        }

        // Precompute suffix minimums and maximums
        // suffix_min[j] stores the minimum value in arr[j...n-1]
        // suffix_max[j] stores the maximum value in arr[j...n-1]
        vector<int> suffix_min(n);
        vector<int> suffix_max(n);
        suffix_min[n-1] = arr[n-1];
        suffix_max[n-1] = arr[n-1];
        for (int i = n - 2; i >= 0; --i) {
            suffix_min[i] = min(suffix_min[i+1], arr[i]);
            suffix_max[i] = max(suffix_max[i+1], arr[i]);
        }

        int min_overall_diff = INT_MAX;

        // Iterate through all possible starting positions `i` of the subarray to be removed.
        // The subarray to be removed is `arr[i...i+k-1]`.
        // `i` can range from `0` to `n - k`.
        for (int i = 0; i <= n - k; ++i) {
            int current_min_val;
            int current_max_val;

            // The remaining elements are `arr[0...i-1]` (prefix) and `arr[i+k...n-1]` (suffix).

            // Case 1: Only suffix remains (removed subarray is at the beginning, i.e., i = 0)
            // The remaining part is `arr[k...n-1]`.
            if (i == 0) {
                current_min_val = suffix_min[k];
                current_max_val = suffix_max[k];
            }
            // Case 2: Only prefix remains (removed subarray is at the end, i.e., i + k = n)
            // The remaining part is `arr[0...n-k-1]`.
            else if (i + k == n) {
                current_min_val = prefix_min[i-1];
                current_max_val = prefix_max[i-1];
            }
            // Case 3: Both prefix and suffix remain (removed subarray is in the middle)
            // The remaining parts are `arr[0...i-1]` and `arr[i+k...n-1]`.
            else { // i > 0 and i + k < n
                current_min_val = min(prefix_min[i-1], suffix_min[i+k]);
                current_max_val = max(prefix_max[i-1], suffix_max[i+k]);
            }
            
            min_overall_diff = min(min_overall_diff, current_max_val - current_min_val);
        }

        return min_overall_diff;
    }
};